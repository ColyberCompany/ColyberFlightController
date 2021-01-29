/**
 * @file RemoteControlComm.h
 * @author Jan Wielgus
 * @brief This (Singleton) class is used to communicate with remote controller.
 * Way of communication could be changed here. This class also contains 
 * variables that store received data or variables where sent data should be placed.
 * // TODO: maybe write something more after implementing this class.
 * @date 2020-11-09
 * 
 */

#ifndef REMOTECONTROLCOMM_H
#define REMOTECONTROLCOMM_H

#include <StreamComm.h>
#include <PacketCommunication.h>
#include <DataPacket.h>
#include "ReceiveData.h"
#include "SendData.h"
#include "PacketReceivedEvents.h"


/**
 * @brief Remote Controller Communication
 * (communication with the remote controller).
 */
class RemoteControlComm
{
public:
    struct SendStuff
    {
        DataForRemoteControl data;

        DataPacket measurementsAndState;
        // add other here ...

        SendStuff()
            : measurementsAndState(10)
        {}
    } sendStuff;


    struct ReceiveStuff
    {
        DataFromRemoteControl data;

        DataPacket steering;
        // add other here ...

        ReceiveStuff()
            : steering(0)
        {}
    } receiveStuff;
    

private:
    SteeringReceivedEvent steeringReceivedEvent;




public:
    RemoteControlComm(PacketCommunication& packetComm)
    {
        // - add all data that this packet consists of
        // - set packet received events for received packets
        // - add receive data packets pointers to the packetComm

    // receive:

        // steering
        receiveStuff.steering.addByteType(receiveStuff.data.throttle);
        receiveStuff.steering.addByteType(receiveStuff.data.yaw);
        receiveStuff.steering.addByteType(receiveStuff.data.pitch);
        receiveStuff.steering.addByteType(receiveStuff.data.roll);
        receiveStuff.steering.setPacketReceivedEvent(steeringReceivedEvent);


    // send:

        // measurements and state
        // TODO: think about structure of this packet and if to split it
        sendStuff.measurementsAndState.addByteType(sendStuff.data.pitchAngle_deg);
        sendStuff.measurementsAndState.addByteType(sendStuff.data.rollAngle_deg);
        sendStuff.measurementsAndState.addByteType(sendStuff.data.heading_deg);
        sendStuff.measurementsAndState.addByteType(sendStuff.data.altitude_cm);
        sendStuff.measurementsAndState.addByteType(sendStuff.data.longitude);
        sendStuff.measurementsAndState.addByteType(sendStuff.data.latitude);
        sendStuff.measurementsAndState.addByteType(sendStuff.data.droneConnectionStability);
        packetComm.addReceiveDataPacketPointer(&receiveStuff.steering);
    }


    RemoteControlComm(const RemoteControlComm&) = delete;
    RemoteControlComm& operator=(const RemoteControlComm&) = delete;
};


#endif
