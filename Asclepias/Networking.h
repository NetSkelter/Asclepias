/*
* File:		Networking.h
* Author:	NetSkelter (modified source from OLC, see licenses directory).
* Created:	2022.11.24
*/

#ifndef ASC_NETWORKING_H
#define ASC_NETWORKING_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <iostream>
#include <mutex>
#include <deque>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "Logging.h"

namespace ASC {
	enum NetMsgTypes : unsigned int {
		ASC_NET_CONNECTED = 0xFFFFFFF0,
		ASC_NET_DISCONNECTED = 0xFFFFFFF1,
		ASC_NET_FAILED = 0xFFFFFFF2,
	};

	class NetMsgHeader {
	public:
		unsigned int type = 0;
		unsigned int size = 0;
	};

	class NetConn;
	typedef std::shared_ptr<NetConn> NetConnPtr;

	class NetMsg {
	public:
		NetConnPtr owner = 0;
		NetMsgHeader header;
		std::vector<uint8_t> body;

		NetMsg() {}
		NetMsg(unsigned int, NetConnPtr = 0);
		friend std::ostream& operator << (std::ostream& os, const NetMsg& msg) {
			os << "NetMsg of type " << msg.header.type << ", " << msg.header.size << " bytes";
			return os;
		}
		template <typename T>
		friend NetMsg& operator << (NetMsg& msg, const T& data) {
			if (std::is_standard_layout<T>::value) {
				size_t s = msg.body.size();
				msg.body.resize(msg.body.size() + sizeof(T));
				std::memcpy(msg.body.data() + s, &data, sizeof(T));
				msg.header.size = (unsigned int)msg.body.size();
			}
			return msg;
		}
		template <typename T>
		friend NetMsg& operator >> (NetMsg& msg, T& data) {
			if (std::is_standard_layout<T>::value) {
				size_t s = msg.body.size() - sizeof(T);
				std::memcpy(&data, msg.body.data() + s, sizeof(T));
				msg.body.resize(s);
				msg.header.size = (unsigned int)msg.body.size();
			}
			return msg;
		}
	};

	template <typename T>
	class TSQueue {
	public:
		TSQueue() = default;
		TSQueue(const TSQueue<T>&) = delete;
		~TSQueue() {
			clear();
		}
		void clear() {
			std::scoped_lock(mtx_);
			dequeue_.clear();
		}
		unsigned int size() const {
			std::scoped_lock(mtx_);
			return (unsigned int)dequeue_.size();
		}
		bool empty() {
			std::scoped_lock(mtx_);
			return dequeue_.empty();
		}
		T& at(unsigned int i) {
			std::scoped_lock(mtx_);
			return dequeue_.at(i);
		}
		const T& front() const {
			std::scoped_lock(mtx_);
			return dequeue_.front();
		}
		const T& back() const {
			std::scoped_lock(mtx_);
			return dequeue_.back();
		}
		T popFront() {
			std::scoped_lock(mtx_);
			T e = std::move(dequeue_.front());
			dequeue_.pop_front();
			return e;
		}
		T popBack() {
			std::scoped_lock(mtx_);
			T e = std::move(dequeue_.back());
			dequeue_.pop_back();
			return e;
		}
		void pushFront(const T& e) {
			std::scoped_lock(mtx_);
			dequeue_.emplace_front(std::move(e));
		}
		void pushBack(const T& e) {
			std::scoped_lock(mtx_);
			dequeue_.emplace_front(std::move(e));
		}

	private:
		std::deque<T> dequeue_;
		std::mutex mtx_;
	};

	class NetConn : public std::enable_shared_from_this<NetConn> {
	public:
		NetConn(asio::io_context& context, asio::ip::tcp::socket& socket) : context_(context),
			socket_(std::move(socket)) {}
		void init(TSQueue<NetMsg>&);
		void connectToServer(const asio::ip::tcp::resolver::results_type&);
		void connectToClient(unsigned int);
		bool isConnected();
		inline unsigned int getID() const {
			return ID_;
		}
		void send(const NetMsg&);
		void disconnect();
		void destroy();

	private:
		asio::io_context& context_;
		asio::ip::tcp::socket socket_;
		bool connected_ = false;
		TSQueue<NetMsg>* msgsIn_ = 0;
		TSQueue<NetMsg> msgsOut_;
		NetMsg tmpMsg_;
		bool serverConn_ = false;
		unsigned int ID_ = 0;

		void readHeader();
		void readBody();
		void addMsg();
		void writeHeader();
		void writeBody();
	};

	class NetClient {
	public:
		NetClient() : socket_(context_) {}
		void init();
		void connect(const std::string&, unsigned short);
		void send(const NetMsg&);
		inline TSQueue<NetMsg>& getMsgs() {
			return msgs_;
		}
		inline bool isConnected() const {
			return connection_->isConnected();
		}
		void disconnect();
		void destroy();

	private:
		asio::io_context context_;
		std::thread asioThread_;
		asio::ip::tcp::socket socket_;
		NetConnPtr connection_ = 0;
		TSQueue<NetMsg> msgs_;
	};

#define NSLOG(SRC, LVL, ...) log_, log_.getTimestamp(), ": [", #SRC, \
	"] [", #LVL, "] ", __VA_ARGS__, "\n"

	class NetServer {
	public:
		NetServer(unsigned short port) : listener_(context_,
			asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), port_(port) {}
		bool init();
		void update(int = -1);
		std::vector<unsigned int> getClientIDs() const;
		NetConnPtr getClientByID(unsigned int);
		inline unsigned int getClientCount() const {
			return (unsigned int)getClientIDs().size();
		}
		bool send(const NetMsg&, NetConnPtr);
		void sendAll(const NetMsg&, NetConnPtr = 0);
		inline bool isConnected() const {
			return listener_.is_open();
		}
		void destroy();

	protected:
		LogMgr log_;
		TSQueue<NetMsg> msgs_;

		virtual bool clientConnected(NetConnPtr) {
			return true;
		}
		virtual void msgReceived(NetMsg&) {}
		virtual void clientDisconnected(NetConnPtr) {}

	private:
		asio::io_context context_;
		std::thread asioThread_;
		unsigned short port_ = 0;
		asio::ip::tcp::acceptor listener_;
		std::deque<NetConnPtr> clients_;
		unsigned int nextID_ = 0;

		void acceptClient();
	};
}

#endif