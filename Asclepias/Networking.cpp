/*
* File:		Networking.cpp
* Author:	NetSkelter (modified code from OLC, see licenses directory).
* Created:	2022.11.24
*/

#include "Asclepias.h"

namespace ASC {
	// Define NetMsg functions.

	NetMsg::NetMsg(unsigned int type, NetConnPtr owner) {
		header.type = type;
		this->owner = owner;
	}

	// Define NetConn functions.

	void NetConn::init(TSQueue<NetMsg>& msgsIn) {
		msgsIn_ = &msgsIn;
		tmpMsg_.owner = this->shared_from_this();
	}

	void NetConn::connectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
		asio::async_connect(socket_, endpoints,
			[this](std::error_code ec, asio::ip::tcp::endpoint ep) {
				if (!ec) {
					connected_ = true;
					NetMsg msg(ASC_NET_CONNECTED, this->shared_from_this());
					msgsIn_->pushBack(msg);
					readHeader();
				}
				else {
					NetMsg msg(ASC_NET_FAILED, this->shared_from_this());
					msgsIn_->pushBack(msg);
					disconnect();
				}
			}
		);
	}
	
	void NetConn::connectToClient(unsigned int ID) {
		if (socket_.is_open()) {
			ID_ = ID;
			connected_ = true;
			serverConn_ = true;
			readHeader();
		}
	}

	bool NetConn::isConnected() {
		if (!socket_.is_open()) {
			connected_ = false;
		}
		else {
			connected_ = true;
		}
		return connected_;
	}

	void NetConn::send(const NetMsg& msg) {
		asio::post(context_,
			[this, msg]() {
				bool outEmpty = msgsOut_.empty();
				msgsOut_.pushBack(msg);
				if (outEmpty) {
					writeHeader();
				}
			}
		);
	}
	
	void NetConn::disconnect() {
		if (isConnected()) {
			connected_ = false;
			if (!serverConn_ && msgsIn_ != 0) {
				NetMsg msg(ASC_NET_DISCONNECTED, this->shared_from_this());
				msgsIn_->pushBack(msg);
			}
			asio::post(context_,
				[this]() {
					socket_.close();
				}
			);
		}
	}

	void NetConn::destroy() {
		disconnect();
		for (unsigned int i = 0; i < msgsIn_->size(); i++) {
			msgsIn_->at(i).owner.reset();
		}
		msgsIn_->clear();
		msgsIn_ = 0;
		for (unsigned int i = 0; i < msgsOut_.size(); i++) {
			msgsOut_.at(i).owner.reset();
		}
		msgsOut_.clear();
		tmpMsg_.owner.reset();
		tmpMsg_.header.type = 0;
		tmpMsg_.header.size = 0;
		tmpMsg_.body.clear();
		serverConn_ = false;
		ID_ = 0;
	}

#define ASC_MAX_NET_MSG_SIZE 1024

	void NetConn::readHeader() {
		asio::async_read(socket_,
			asio::buffer(&tmpMsg_.header, sizeof(NetMsgHeader)),
			[this](std::error_code ec, std::size_t len) {
				if (!ec) {
					if (tmpMsg_.header.size == 0) {
						addMsg();
					}
					else {
						if (tmpMsg_.header.size <= ASC_MAX_NET_MSG_SIZE) {
							tmpMsg_.body.resize(tmpMsg_.header.size);
							readBody();
						}
						else {
							disconnect();
						}
					}
				}
				else {
					disconnect();
				}
			}
		);
	}

	void NetConn::readBody() {
		asio::async_read(socket_,
			asio::buffer(tmpMsg_.body.data(), tmpMsg_.body.size()),
			[this](std::error_code ec, std::size_t len) {
				if (!ec) {
					addMsg();
				}
				else {
					disconnect();
				}
			}
		);
	}

	void NetConn::addMsg() {
		msgsIn_->pushBack(tmpMsg_);
		tmpMsg_.header.type = 0;
		tmpMsg_.header.size = 0;
		tmpMsg_.body.clear();
		readHeader();
	}

	void NetConn::writeHeader() {
		asio::async_write(socket_,
			asio::buffer(&msgsOut_.front().header, sizeof(NetMsgHeader)),
			[this](std::error_code ec, std::size_t len) {
				if (!ec) {
					if (msgsOut_.front().header.size == 0) {
						msgsOut_.popFront();
						if (!msgsOut_.empty()) {
							writeHeader();
						}
					}
					else {
						if (msgsOut_.front().header.size <= ASC_MAX_NET_MSG_SIZE) {
							writeBody();
						}
						else {
							disconnect();
						}
					}
				}
				else {
					disconnect();
				}
			}
		);
	}

	void NetConn::writeBody() {
		asio::async_write(socket_,
			asio::buffer(msgsOut_.front().body.data(), msgsOut_.front().body.size()),
			[this](std::error_code ec, std::size_t len) {
				if (!ec) {
					msgsOut_.popFront();
					if (!msgsOut_.empty()) {
						writeHeader();
					}
				}
				else {
					disconnect();
				}
			}
		);
	}

	// Define NetClient functions.

	void NetClient::init() {
		connection_ = std::make_shared<NetConn>(context_, socket_);
		connection_->init(msgs_);
	}

	void NetClient::connect(const std::string& addr, unsigned short port) {
		std::cout << "Connecting to " << addr << ":" << port << "." << std::endl;
		disconnect();
		std::cout << "Attempting connection." << std::endl;
		try {
			asio::ip::tcp::resolver resolver(context_);
			std::cout << "Set up resolver." << std::endl;
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(addr, std::to_string(port));
			std::cout << "Retrieved endpoints." << std::endl;
			connection_->connectToServer(endpoints);
			std::cout << "Connected connection to server." << std::endl;
			asioThread_ = std::thread(
				[this]() {
					context_.run();
				}
			);
			std::cout << "Started thread." << std::endl;
		}
		catch (std::exception&) {
			std::cout << "Exception hit." << std::endl;
			disconnect();
		}
	}

	void NetClient::send(const NetMsg& msg) {
		if (isConnected()) {
			connection_->send(msg);
		}
		else {
			disconnect();
		}
	}

	void NetClient::disconnect() {
		std::cout << "Disconnecting." << std::endl;
		connection_->disconnect();
		std::cout << "Disconnected connection." << std::endl;
		context_.stop();
		std::cout << "Stopped context." << std::endl;
		context_.reset();
		std::cout << "Reset context." << std::endl;
		if (asioThread_.joinable()) {
			std::cout << "Stopping thread." << std::endl;
			asioThread_.join();
		}
	}

	void NetClient::destroy() {
		disconnect();
		connection_->destroy();
		connection_.reset();
		msgs_.clear();
	}

	// Define NetServer functions.

	bool NetServer::init() {
		log_.init(true, { "ASCSRV.log" }, "%Y.%m.%d.%H%M.%S");
		NSLOG(Init, Info, "Initializing ASC server on port ", port_, ".");
		try {
			acceptClient();
			asioThread_ = std::thread(
				[this]() {
					context_.run();
				}
			);
		}
		catch (std::exception& e) {
			const char* err = e.what();
			NSLOG(Init, Error, "Failed to open server:\n", err);
			destroy();
			return false;
		}
		return true;
	}

	void NetServer::update(int count) {
		for (NetConnPtr client : clients_) {
			if (client) {
				if (client->isConnected()) {
					continue;
				}
				else {
					NSLOG(Update, Info, "Client ", client->getID(), " has disconnected.");
					clientDisconnected(client);
					client->destroy();
				}
			}
			clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
		}
		if (count < 0) {
			while (!msgs_.empty()) {
				NetMsg msg = msgs_.popFront();
				msgReceived(msg);
			}
		}
		else {
			for (int i = 0; i < count; i++) {
				NetMsg msg = msgs_.popFront();
				msgReceived(msg);
			}
		}
	}

	std::vector<unsigned int> NetServer::getClientIDs() const {
		std::vector<unsigned int> IDs;
		for (NetConnPtr client : clients_) {
			IDs.push_back(client->getID());
		}
		return IDs;
	}

	NetConnPtr NetServer::getClientByID(unsigned int ID) {
		for (NetConnPtr client : clients_) {
			if (client->getID() == ID) {
				return client;
			}
		}
		return 0;
	}

	bool NetServer::send(const NetMsg& msg, NetConnPtr client) {
		if (client) {
			if (client->isConnected()) {
				client->send(msg);
				return true;
			}
		}
		NSLOG(Send, Warning, "Failed to send ", msg, " to client ", client->getID(), ".");
		clientDisconnected(client);
		client->destroy();
		clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
		return false;
	}

	void NetServer::sendAll(const NetMsg& msg, NetConnPtr ignored) {
		for (NetConnPtr client : clients_) {
			if (client != ignored) {
				send(msg, client);
			}
		}
	}

	void NetServer::destroy() {
		NSLOG(Destroy, Info, "Stopping ASC server.");
		for (NetConnPtr client : clients_) {
			client->destroy();
		}
		clients_.clear();
		NSLOG(Destroy, Info, "Disconnected all clients.");
		asio::post(context_,
			[this]() {
				listener_.cancel();
				listener_.close();
			}
		);
		context_.stop();
		asioThread_.join();
		context_.reset();
		NSLOG(Destroy, Info, "Stopped ASC client.");
		log_.destroy();
		port_ = 0;
		nextID_ = 0;
	}

	void NetServer::acceptClient() {
		NSLOG(Accept, Info, "Waiting for new client.");
		listener_.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket) {
				if (!ec) {
					NSLOG(Accept, Info, "Found new client, assigned ID ", nextID_, ".");
					NetConnPtr client = std::make_shared<NetConn>(context_, socket);
					if (clientConnected(client)) {
						NSLOG(Accept, Info, "Accepted client ", client->getID(), ".");
						client->init(msgs_);
						client->connectToClient(nextID_++);
						clients_.push_back(client);
					}
					else {
						NSLOG(Accept, Info, "Rejected client ", client->getID(), ".");
						socket.close();
					}
					acceptClient();
				}
				else {
					NSLOG(Accept, Error, "Failed to accept client.");
					destroy();
				}
			}
		);
	}
}