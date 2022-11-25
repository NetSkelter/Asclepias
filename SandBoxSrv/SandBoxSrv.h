/*
* File:		SandBoxSrv.h
* Author:	NetSkelter
* Created:	2022.11.25
*/

#ifndef ASC_SAND_BOX_SRV_H
#define ASC_SAND_BOX_SRV_H

#include <Asclepias/Networking.h>

using namespace ASC;

class SandBoxSrv : public NetServer {
public:
	SandBoxSrv(unsigned short);

protected:
	bool clientConnected(NetConnPtr) override;
	void msgReceived(NetMsg&) override;
	void clientDisconnected(NetConnPtr) override;
};

#endif