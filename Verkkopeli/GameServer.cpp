#include <Book/GameServer.hpp>
#include <Book/NetworkProtocol.hpp>
#include <Book/Foreach.hpp>
#include <Book/Utility.hpp>
#include <Book/Pickup.hpp>
#include <Book/PlayerBat.hpp>

#include <SFML/Network/Packet.hpp>

GameServer::RemotePeer::RemotePeer() 
: ready(false)
, timedOut(false)
{
	socket.setBlocking(false);
}

GameServer::GameServer(sf::Vector2f battlefieldSize)
: mThread(&GameServer::executionThread, this)
, mListeningState(false)
, mClientTimeoutTime(sf::seconds(3.f))
, mMaxConnectedPlayers(10)
, mConnectedPlayers(0)
, mWorldHeight(battlefieldSize.y)
, mPlayAreaRect(0.f, battlefieldSize.y, battlefieldSize.x, battlefieldSize.y)
, mPlayerBatCount(0)
, mPeers(1)
, mPlayerBatIdentifierCounter(1)
, mWaitingThreadEnd(false)
, mLastSpawnTime(sf::Time::Zero)
, mTimeForNextSpawn(sf::seconds(5.f))
{
	mListenerSocket.setBlocking(false);
	mPeers[0].reset(new RemotePeer());
	mThread.launch();
}

GameServer::~GameServer()
{
	mWaitingThreadEnd = true;
	mThread.wait();
}

void GameServer::notifyPlayerRealtimeChange(sf::Int32 PlayerBatIdentifier, sf::Int32 action, bool actionEnabled)
{
	for (std::size_t i = 0; i < mConnectedPlayers; ++i)
	{
		if (mPeers[i]->ready)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Server::PlayerRealtimeChange);
			packet << PlayerBatIdentifier;
			packet << action;
			packet << actionEnabled;

			mPeers[i]->socket.send(packet);
		}
	}
}

void GameServer::notifyPlayerEvent(sf::Int32 PlayerBatIdentifier, sf::Int32 action)
{
	for (std::size_t i = 0; i < mConnectedPlayers; ++i)
	{
		if (mPeers[i]->ready)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Server::PlayerEvent);
			packet << PlayerBatIdentifier;
			packet << action;

			mPeers[i]->socket.send(packet);
		}
	}
}

void GameServer::notifyPlayerSpawn(sf::Int32 PlayerBatIdentifier)
{
	for (std::size_t i = 0; i < mConnectedPlayers; ++i)
	{
		if (mPeers[i]->ready)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Server::PlayerConnect);
			packet << PlayerBatIdentifier << mPlayerBatInfo[PlayerBatIdentifier].position.x << mPlayerBatInfo[PlayerBatIdentifier].position.y;
			mPeers[i]->socket.send(packet);
		}
	}
}

void GameServer::setListening(bool enable)
{
	// Check if it isn't already listening
	if (enable)
	{	
		if (!mListeningState)
			mListeningState = (mListenerSocket.listen(ServerPort) == sf::TcpListener::Done);
	}
	else
	{
		mListenerSocket.close();
		mListeningState = false;
	}
}

void GameServer::executionThread()
{
	setListening(true);

	sf::Time stepInterval = sf::seconds(1.f / 60.f);
	sf::Time stepTime = sf::Time::Zero;
	sf::Time tickInterval = sf::seconds(1.f / 20.f);
	sf::Time tickTime = sf::Time::Zero;
	sf::Clock stepClock, tickClock;

	while (!mWaitingThreadEnd)
	{	
		handleIncomingPackets();
		handleIncomingConnections();

		stepTime += stepClock.getElapsedTime();
		stepClock.restart();

		tickTime += tickClock.getElapsedTime();
		tickClock.restart();

		// Fixed update step
		while (stepTime >= stepInterval)
		{
			stepTime -= stepInterval;
		}

		// Fixed tick step
		while (tickTime >= tickInterval)
		{
			tick();
			tickTime -= tickInterval;
		}

		// Sleep to prevent server from consuming 100% CPU
		sf::sleep(sf::milliseconds(100));
	}	
}

void GameServer::tick()
{
	updateClientState();

	// Check for mission success = all planes with position.y < offset
	bool allPlayerBatsDone = true;
	FOREACH(auto pair, mPlayerBatInfo)
	{
		// As long as one player has not crossed the finish line yet, set variable to false
		if (pair.second.position.y > 0.f)
			allPlayerBatsDone = false;
	}
	if (allPlayerBatsDone)
	{
		sf::Packet missionSuccessPacket;
		missionSuccessPacket << static_cast<sf::Int32>(Server::MissionSuccess);
		sendToAll(missionSuccessPacket);
	}

	// Remove IDs of PlayerBat that have been destroyed (relevant if a client has two, and loses one)
	/*for (auto itr = mPlayerBatInfo.begin(); itr != mPlayerBatInfo.end(); )
	{
		if (itr->second.hitpoints <= 0)
			mPlayerBatInfo.erase(itr++);
		else
			++itr;
	}*/
}

sf::Time GameServer::now() const
{
	return mClock.getElapsedTime();
}

void GameServer::handleIncomingPackets()
{
	bool detectedTimeout = false;
	
	FOREACH(PeerPtr& peer, mPeers)
	{
		if (peer->ready)
		{
			sf::Packet packet;
			while (peer->socket.receive(packet) == sf::Socket::Done)
			{
				// Interpret packet and react to it
				handleIncomingPacket(packet, *peer, detectedTimeout);

				// Packet was indeed received, update the ping timer
				peer->lastPacketTime = now();
				packet.clear();
			}

			if (now() >= peer->lastPacketTime + mClientTimeoutTime)
			{
				peer->timedOut = true;
				detectedTimeout = true;
			}
		}
	}

	if (detectedTimeout)
		handleDisconnections();
}

void GameServer::handleIncomingPacket(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout)
{
	sf::Int32 packetType;
	packet >> packetType;

	switch (packetType)
	{
		case Client::Quit:
		{
			receivingPeer.timedOut = true;
			detectedTimeout = true;
		} break;

		case Client::PlayerEvent:
		{
			sf::Int32 PlayerBatIdentifier;
			sf::Int32 action;
			packet >> PlayerBatIdentifier >> action;

			notifyPlayerEvent(PlayerBatIdentifier, action);
		} break;

		case Client::PlayerRealtimeChange:
		{
			sf::Int32 PlayerBatIdentifier;
			sf::Int32 action;
			bool actionEnabled;
			packet >> PlayerBatIdentifier >> action >> actionEnabled;
			mPlayerBatInfo[PlayerBatIdentifier].realtimeActions[action] = actionEnabled;
			notifyPlayerRealtimeChange(PlayerBatIdentifier, action, actionEnabled);
		} break;

		case Client::RequestCoopPartner:
		{
			receivingPeer.playerBatIdentifiers.push_back(mPlayerBatIdentifierCounter);
			mPlayerBatInfo[mPlayerBatIdentifierCounter].position = sf::Vector2f(mPlayAreaRect.width / 2, mPlayAreaRect.top + mPlayAreaRect.height / 2);

			sf::Packet requestPacket;
			requestPacket << static_cast<sf::Int32>(Server::AcceptCoopPartner);
			requestPacket << mPlayerBatIdentifierCounter;
			requestPacket << mPlayerBatInfo[mPlayerBatIdentifierCounter].position.x;
			requestPacket << mPlayerBatInfo[mPlayerBatIdentifierCounter].position.y;

			receivingPeer.socket.send(requestPacket);
			mPlayerBatCount++;

			// Inform every other peer about this new plane
			FOREACH(PeerPtr& peer, mPeers)
			{
				if (peer.get() != &receivingPeer && peer->ready)
				{
					sf::Packet notifyPacket;
					notifyPacket << static_cast<sf::Int32>(Server::PlayerConnect);
					notifyPacket << mPlayerBatIdentifierCounter;
					notifyPacket << mPlayerBatInfo[mPlayerBatIdentifierCounter].position.x;
					notifyPacket << mPlayerBatInfo[mPlayerBatIdentifierCounter].position.y;
					peer->socket.send(notifyPacket);
				}
			}
			mPlayerBatIdentifierCounter++;
		} break;

		case Client::PositionUpdate:
		{
			sf::Int32 numPlayerBats;
			packet >> numPlayerBats;

			for (sf::Int32 i = 0; i < numPlayerBats; ++i)
			{
				sf::Int32 PlayerBatIdentifier;
				sf::Vector2f PlayerBatPosition;
				packet >> PlayerBatIdentifier >> PlayerBatPosition.x >> PlayerBatPosition.y;
				mPlayerBatInfo[PlayerBatIdentifier].position = PlayerBatPosition;
			}
		} break;

		case Client::GameEvent:
		{
			sf::Int32 action;
			float x;
			float y;

			packet >> action;
			packet >> x;
			packet >> y;

			// Enemy explodes: With certain probability, drop pickup
			// To avoid multiple messages spawning multiple pickups, only listen to first peer (host)
			if (action == GameActions::EnemyExplode && randomInt(3) == 0 && &receivingPeer == mPeers[0].get())
			{
				sf::Packet packet;
				packet << static_cast<sf::Int32>(Server::SpawnPickup);
				packet << static_cast<sf::Int32>(randomInt(Pickup::TypeCount));
				packet << x;
				packet << y;

				sendToAll(packet);
			}
		}
	}
}

void GameServer::updateClientState()
{
	sf::Packet updateClientStatePacket;
	updateClientStatePacket << static_cast<sf::Int32>(Server::UpdateClientState);
	updateClientStatePacket << static_cast<float>(mPlayAreaRect.top + mPlayAreaRect.height);
	updateClientStatePacket << static_cast<sf::Int32>(mPlayerBatInfo.size());

	FOREACH(auto PlayerBat, mPlayerBatInfo)
		updateClientStatePacket << PlayerBat.first << PlayerBat.second.position.x << PlayerBat.second.position.y;

	sendToAll(updateClientStatePacket);
}

void GameServer::handleIncomingConnections()
{
	if (!mListeningState)
		return;

	if (mListenerSocket.accept(mPeers[mConnectedPlayers]->socket) == sf::TcpListener::Done)
	{
		// order the new client to spawn its own plane ( player 1 )
		mPlayerBatInfo[mPlayerBatIdentifierCounter].position = sf::Vector2f(mPlayAreaRect.width / 2, mPlayAreaRect.top + mPlayAreaRect.height / 2);

		sf::Packet packet;
		packet << static_cast<sf::Int32>(Server::SpawnSelf);
		packet << mPlayerBatIdentifierCounter;
		packet << mPlayerBatInfo[mPlayerBatIdentifierCounter].position.x;
		packet << mPlayerBatInfo[mPlayerBatIdentifierCounter].position.y;
		
		mPeers[mConnectedPlayers]->playerBatIdentifiers.push_back(mPlayerBatIdentifierCounter);
		
		broadcastMessage("New player!");
		informWorldState(mPeers[mConnectedPlayers]->socket);
		notifyPlayerSpawn(mPlayerBatIdentifierCounter++);

		mPeers[mConnectedPlayers]->socket.send(packet);
		mPeers[mConnectedPlayers]->ready = true;
		mPeers[mConnectedPlayers]->lastPacketTime = now(); // prevent initial timeouts
		mPlayerBatCount++;
		mConnectedPlayers++;

		if (mConnectedPlayers >= mMaxConnectedPlayers)
			setListening(false);
		else // Add a new waiting peer
			mPeers.push_back(PeerPtr(new RemotePeer()));
	}
}

void GameServer::handleDisconnections()
{
	for (auto itr = mPeers.begin(); itr != mPeers.end(); )
	{
		if ((*itr)->timedOut)
		{
			// Inform everyone of the disconnection, erase 
			FOREACH(sf::Int32 identifier, (*itr)->playerBatIdentifiers)
			{
				sendToAll(sf::Packet() << static_cast<sf::Int32>(Server::PlayerDisconnect) << identifier);

				mPlayerBatInfo.erase(identifier);
			}

			mConnectedPlayers--;
			mPlayerBatCount -= (*itr)->playerBatIdentifiers.size();

			itr = mPeers.erase(itr);

			// Go back to a listening state if needed
			if (mConnectedPlayers < mMaxConnectedPlayers)
			{
				mPeers.push_back(PeerPtr(new RemotePeer()));
				setListening(true);
			}
				
			broadcastMessage("An ally has disconnected.");
		}
		else
		{
			++itr;
		}
	}
}

// Tell the newly connected peer about how the world is currently
void GameServer::informWorldState(sf::TcpSocket& socket)
{
	sf::Packet packet;
	packet << static_cast<sf::Int32>(Server::InitialState);
	packet << mWorldHeight;
	sf::Int32 playerBatCount = static_cast<sf::Int32>(mPlayerBatCount);
	packet << playerBatCount;

	for (std::size_t i = 0; i < mConnectedPlayers; ++i)
	{
		if (mPeers[i]->ready)
		{
			FOREACH(sf::Int32 identifier, mPeers[i]->playerBatIdentifiers)
				packet << identifier << mPlayerBatInfo[identifier].position.x << mPlayerBatInfo[identifier].position.y;
		}
	}

	socket.send(packet);
}

void GameServer::broadcastMessage(const std::string& message)
{
	for (std::size_t i = 0; i < mConnectedPlayers; ++i)
	{
		if (mPeers[i]->ready)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int32>(Server::BroadcastMessage);
			packet << message;

			mPeers[i]->socket.send(packet);
		}	
	}
}

void GameServer::sendToAll(sf::Packet& packet)
{
	FOREACH(PeerPtr& peer, mPeers)
	{
		if (peer->ready)
			peer->socket.send(packet);
	}
}
