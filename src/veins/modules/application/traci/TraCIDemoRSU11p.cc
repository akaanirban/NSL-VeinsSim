//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"
#include <vector>
#include <cstdlib>
#include <ctime>

Define_Module(TraCIDemoRSU11p);

void TraCIDemoRSU11p::initialize(int stage){ //added anirban
    BaseWaveApplLayer::initialize(stage);
    if (stage ==0){
        cout<< "Initializing RSU: "<<getParentModule()->getIndex()<<" Is Hotspot? "<<par("hotspotFlag").boolValue()<<endl;
        BaseWaveApplLayer::prepareRSUmessage(sendRSUCSWM); //prepare the SEND_RSU_EVT //anirban code
    }
    if(stage==1){
        if (sendRSUMsgs) { //anirban code
            simtime_t randomOffset = dblrand() * RSUMsgInterval;
            simtime_t firstRSU = simTime() + randomOffset;
            scheduleAt(firstRSU, sendRSUCSWM);
       }
    }
}

void TraCIDemoRSU11p::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
    printf("taylor");
}

void TraCIDemoRSU11p::onWSM(WaveShortMessage* wsm) {
    //this rsu repeats the received traffic update in 2 seconds plus some random delay
    wsm->setSenderAddress(myId);
    sendDelayedDown(wsm->dup(), 1 + uniform(0.01,0.2));
    printf("taylor");
}

void TraCIDemoRSU11p::onCWSM(WaveShortMessage* wsm) {  //Updated sim code, custom method to handle receiving a "custom wave short message".  This only prints out the message when received at this time
    // if you want the RSUs to do something on receipt of a CSWM, put it here
    //sendDown(wsm);
    //cout<<"YO YO "<<endl;
}

void TraCIDemoRSU11p::handleSelfMsg(cMessage* msg) {

    if (WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg)) {
        if(wsm->getPsid()==-10){
            sendDown(wsm->dup());
            scheduleAt(simTime() + RSUMsgInterval+uniform(0.02, 0.05), sendRSUCSWM);
            //sendDelayedDown(sendRSUCSWM, simTime()+RSUMsgInterval+uniform(0.02, 0.05));
        }
    }
    else {
            BaseWaveApplLayer::handleSelfMsg(msg);
    }
}
