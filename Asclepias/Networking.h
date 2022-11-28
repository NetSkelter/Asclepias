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
	// Enumeration of default types for network messages.
	enum NetMsgTypes : unsigned int {
		ASC_NET_CONNECTED = 0xFFFFFFF0,
		ASC_NET_DISCONNECTED = 0xFFFFFFF1,
		ASC_NET_FAILED = 0xFFFFFFF2,
	};
	
	// Header information for a network message.
	class NetMsgHeader {
	public:
		// The type ID number of this message.
		unsigned int type = 0;
		// The size in bytes of this message's body.
		unsigned int size = 0;
	};

	// Forward declare the NetConn class.
	class NetConn;
	// Rename smart pointer for NetConn's.
	typedef std::shared_ptr<NetConn> NetConnPtr;

	// A message containing arbitrary data to send via NetConn.
	class NetMsg {
	public:
		// The connection which owns this message on the local side of the connection.
		NetConnPtr owner = 0;
		// The header information of this message.
		NetMsgHeader header;
		// The data contained in this message.
		std::vector<uint8_t> body;

		/*
		* Construct a blank NetMsg.
		*/
		NetMsg() {}
		/*
		* Construct a NetMsg.
		* @param unsigned int type: The type ID number for this message.
		* @param NetConnPtr owner: The connection which owns this message.
		*/
		NetMsg(unsigned int, NetConnPtr = 0);
		/*
		* Print header information of this message to a stream.
		* @param std::ostream& os: An output stream.
		* @param const NetMsg& msg: The message to print the header of.
		* @return The output stream.
		*/
		friend std::ostream& operator << (std::ostream& os, const NetMsg& msg) {
			os << "NetMsg of type " << msg.header.type << ", " << msg.header.size << " bytes";
			return os;
		}
		/*
		* Copy some data into the body of this message.
		* @param NetMsg& msg: The message to add data to.
		* @param const T& data: The data to add to this message.
		* @return The message.
		*/
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
		/*
		* Pop some data out of the body of this message.
		* @param NetMsg& msg: The message to read data from.
		* @param T& data: The object to read the message into.
		* @return The message.
		*/
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

	/*
	* Thread-safe double-ended queue structure.
	*/
	template <typename T>
	class TSQueue {
	public:
		/*
		* Construct an empty TSQueue.
		*/
		TSQueue() = default;
		/*
		* Do not copy TSQueue's.
		* @param const TSQueue<T>& q: The queue not to copy.
		*/
		TSQueue(const TSQueue<T>&) = delete;
		/*
		* Free the memory of a TSQueue.
		*/
		~TSQueue() {
			clear();
		}
		/*
		* Clear this TSQueue's elements.
		*/
		void clear() {
			std::scoped_lock(mtx_);
			dequeue_.clear();
		}
		/*
		* @return The number of elements in this TSQueue.
		*/
		unsigned int size() const {
			std::scoped_lock(mtx_);
			return (unsigned int)dequeue_.size();
		}
		/*
		* @return Whether this TSQueue contains no elements.
		*/
		bool empty() {
			std::scoped_lock(mtx_);
			return dequeue_.empty();
		}
		/*
		* Get the element of this TSQueue at the given index.
		* @param unsigned int i: The index to get.
		* @return The element of this TSQueue at the index i.
		*/
		T& at(unsigned int i) {
			std::scoped_lock(mtx_);
			return dequeue_.at(i);
		}
		/*
		* @return The element at the front of this TSQueue.
		*/
		const T& front() const {
			std::scoped_lock(mtx_);
			return dequeue_.front();
		}
		/*
		* @return The element at the back of this TSQueue.
		*/
		const T& back() const {
			std::scoped_lock(mtx_);
			return dequeue_.back();
		}
		/*
		* Get and remove an element from the front of this TSQueue.
		* @return The element at the front.
		*/
		T popFront() {
			std::scoped_lock(mtx_);
			T e = std::move(dequeue_.front());
			dequeue_.pop_front();
			return e;
		}
		/*
		* Get and remove an element from the back of this TSQueue.
		* @return The element at the back.
		*/
		T popBack() {
			std::scoped_lock(mtx_);
			T e = std::move(dequeue_.back());
			dequeue_.pop_back();
			return e;
		}
		/*
		* Add an element to the front of this TSQueue.
		* @param const T& e: The element to add.
		*/
		void pushFront(const T& e) {
			std::scoped_lock(mtx_);
			dequeue_.emplace_front(std::move(e));
		}
		/*
		* Add an element to the back of this TSQueue.
		* @param const T& e: The element to add.
		*/
		void pushBack(const T& e) {
			std::scoped_lock(mtx_);
			dequeue_.emplace_front(std::move(e));
		}

	private:
		// The double-ended queue this TSQueue wraps.
		std::deque<T> dequeue_;
		// Mutex used to give this TSQueue thread-safety.
		std::mutex mtx_;
	};

	// Abstract TCP network socket connection.
	class NetConn : public std::enable_shared_from_this<NetConn> {
	public:
		/*
		* Construct a new net connection.
		* @param asio::io_context& context: The ASIO context used to do this connection's work.
		* @param asio::ip::tcp::socket& socket: The TCP socket to use for this connection.
		*/
		NetConn(asio::io_context& context, asio::ip::tcp::socket& socket) : context_(context),
			socket_(std::move(socket)) {}
		/*
		* Initialize this connection.
		* @param TSQueue<NetMsg>& msgsIn: Reference to the queue for this connection to place incoming
		* messages into.
		*/
		void init(TSQueue<NetMsg>&);
		/*
		* Connect this connection's socket to a remote server.
		* @param const asio::ip::tcp::resolver::results_type& endpoints: The server address to resolve.
		*/
		void connectToServer(const asio::ip::tcp::resolver::results_type&);
		/*
		* Connect this connection's socket to a remote client, connection must be owned by a server.
		* @param unsigned int ID: The ID number to assign to this client.
		*/
		void connectToClient(unsigned int);
		/*
		* @return Whether this connection's socket is currently open.
		*/
		bool isConnected();
		/*
		* @return The ID number of this connection, valid only if owned by a server.
		*/
		inline unsigned int getID() const {
			return ID_;
		}
		/*
		* Send a message over this connection's socket.
		* @param const NetMsg& msg: The message to send.
		*/
		void send(const NetMsg&);
		/*
		* Disconnect this connection's socket from the internet.
		*/
		void disconnect();
		/*
		* Free this connection's memory.
		*/
		void destroy();

	private:
		// Reference to the ASIO context used to do this connection's work.
		asio::io_context& context_;
		// TCP socket for this connection to send data over.
		asio::ip::tcp::socket socket_;
		// Whether this connection is currently open.
		bool connected_ = false;
		// Pointer to the queue to place incoming messages into the back of.
		TSQueue<NetMsg>* msgsIn_ = 0;
		// Queue of outgoing messages for this connection to send.
		TSQueue<NetMsg> msgsOut_;
		// Temporary message used to read messages into the incoming queue.
		NetMsg tmpMsg_;
		// Whether this connection is owned by a server.
		bool serverConn_ = false;
		// The ID number of this connection, valid only if owned by a server.
		unsigned int ID_ = 0;

		/*
		* Read the header of the temp message from the socket.
		*/
		void readHeader();
		/*
		* Read the body of the temp message from the socket.
		*/
		void readBody();
		/*
		* Add a copy of the temp message to the incoming messages queue.
		*/
		void addMsg();
		/*
		* Write the header of the next message in the outgoing queue.
		*/
		void writeHeader();
		/*
		* Write the body of the next message in the outgoing queue.
		*/
		void writeBody();
	};

	// Network client based on NetConn.
	class NetClient {
	public:
		/*
		* Construct a new network client.
		*/
		NetClient() : socket_(context_) {}
		/*
		* Initialize this client's memory and ASIO context.
		*/
		void init();
		/*
		* Connect this client to a remote server.
		* @param const std::string& addr: The IP address of the server.
		* @param unsigned short port: The port to connect to the server on.
		*/
		void connect(const std::string&, unsigned short);
		/*
		* Send a message to the server.
		* @param const NetMsg& msg: The message to send.
		*/
		void send(const NetMsg&);
		/*
		* @return The queue of incoming messages to this client.
		*/
		inline TSQueue<NetMsg>& getMsgs() {
			return msgs_;
		}
		/*
		* @return Whether this client is currently connected to a remote server.
		*/
		inline bool isConnected() const {
			return connection_->isConnected();
		}
		/*
		* Disconnet this client from any server.
		*/
		void disconnect();
		/*
		* Free this client's memory.
		*/
		void destroy();

	private:
		// The ASIO context for this client's network work.
		asio::io_context context_;
		// The thread to run this client's ASIO context in.
		std::thread asioThread_;
		// The TCP socket for this client's connection to use.
		asio::ip::tcp::socket socket_;
		// The connection to use to send this client's messages.
		NetConnPtr connection_ = 0;
		// Queue of incoming messages for this client.
		TSQueue<NetMsg> msgs_;
	};

#define NSLOG(SRC, LVL, ...) log_, log_.getTimestamp(), ": [", #SRC, \
	"] [", #LVL, "] ", __VA_ARGS__, "\n"

	// Abstract TCP server interface.
	class NetServer {
	public:
		/*
		* Construct a new network server.
		* @param unsigned short port: The port to listen for clients on.
		*/
		NetServer(unsigned short port) : listener_(context_,
			asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), port_(port) {}
		/*
		* Start this server listening for new clients.
		* @return Whether the server could be opened.
		*/
		bool init();
		/*
		* Read a set of incoming messages into the msgReceived() callback.
		* @param int count: The number of messages to process (-1 for inf).
		*/
		void update(int = -1);
		/*
		* @return The set of currently connected client ID numbers.
		*/
		std::vector<unsigned int> getClientIDs() const;
		/*
		* Get a pointer to a client by their ID number.
		* @param unigned int ID: The ID number of the client on this server.
		* @return A pointer to the client with the given ID or null.
		*/
		NetConnPtr getClientByID(unsigned int);
		/*
		* @return The number of clients currently connected to the server.
		*/
		inline unsigned int getClientCount() const {
			return (unsigned int)getClientIDs().size();
		}
		/*
		* Send a message to a given client.
		* @param const NetMsg& msg: The message to send.
		* @param NetConnPtr client: The client to send the message to.
		* @return Whether the message could be sent.
		*/
		bool send(const NetMsg&, NetConnPtr);
		/*
		* Send a message to all clients on this server except one.
		* @param const NetMsg& msg: The message to send.
		* @param NetConnPtr ignored: A client not to send this message to (null for none).
		*/
		void sendAll(const NetMsg&, NetConnPtr = 0);
		/*
		* @return Whether this server is currently open.
		*/
		inline bool isConnected() const {
			return listener_.is_open();
		}
		/*
		* Stop this server listening for clients and free this server's memory.
		*/
		void destroy();

	protected:
		// The application log for this network server.
		LogMgr log_;
		// Queue of incoming messages from clients.
		TSQueue<NetMsg> msgs_;

		/*
		* A new client has connected to this server.
		* @param NetConnPtr client: The client attempting to connect.
		* @return Whether to accept this client onto the server.
		*/
		virtual bool clientConnected(NetConnPtr) {
			return true;
		}
		/*
		* A client has sent a message to this server.
		* @param NetMsg& msg: The message received.
		*/
		virtual void msgReceived(NetMsg&) {}
		/*
		* A client has disconnected from this server.
		* @param NetConnPtr client: The disconnected client.
		*/
		virtual void clientDisconnected(NetConnPtr) {}

	private:
		// The ASIO context used to do this server's networking work.
		asio::io_context context_;
		// The thread used for this server's ASIO context.
		std::thread asioThread_;
		// The port to listen for this server's clients on.
		unsigned short port_ = 0;
		// ASIO TCP acceptor for getting clients onto the server.
		asio::ip::tcp::acceptor listener_;
		// The set of currently connected clients.
		std::deque<NetConnPtr> clients_;
		// The next ID to assign to a new client.
		unsigned int nextID_ = 0;

		/*
		* Use ASIO to accept a new client onto this server.
		*/
		void acceptClient();
	};
}

#endif