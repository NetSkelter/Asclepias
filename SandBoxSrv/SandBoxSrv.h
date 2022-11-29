/*
* File:		SandBoxSrv.h
* Author:	NetSkelter
* Created:	2022.11.25
*/

#ifndef SBS_SAND_BOX_SRV_H
#define SBS_SAND_BOX_SRV_H

#include <Asclepias/Networking.h>
#include <glm/glm.hpp>

using namespace ASC;

class Player {
public:
	unsigned int ID = 0;
	std::string username = "";
};

class SandBoxSrv : public NetServer {
public:
	SandBoxSrv(unsigned short);

protected:
	bool clientConnected(NetConnPtr) override;
	void msgReceived(NetMsg&) override;
	void clientDisconnected(NetConnPtr) override;

private:
	std::vector<Player> players_;

	void assignUsername(unsigned int, const std::string&);
	std::array<char, 20> convertUsername(const std::string&);
};

#endif