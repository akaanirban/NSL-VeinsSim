//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
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

#include "veins/modules/application/traci/TraCIDemo11p.h"
#include <string>
#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <ctime>


using namespace std;

Define_Module(TraCIDemo11p);

void TraCIDemo11p::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        cout << "Vehicle: "<< mobility->getExternalId() << " initiated"<<endl;
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        aggregatedMessage = new WaveShortMessage(); //anirban code
        string dummyTag(numRSUs, '0');
        dummyTag+=":000";
        populateWSM(aggregatedMessage, 0, 0, dummyTag.c_str());  //create variable to customize message?
        aggregatedMessage->setPsid(-10);
    }
    //mobility = TraCIMobilityAccess().get(mobility);
    WaveShortMessage* wsm = new WaveShortMessage();  //Updated sim code
    //populateWSM(wsm, 0, 0, "Go Bobcats!");  //create variable to customize message?

    string dummyTag(numRSUs, '0');
    dummyTag+=":000";
    populateWSM(wsm, 0, 0, dummyTag.c_str());  //create variable to customize message?
    wsm->setPsid(-10);
    //scheduleAt(simTime() + 1 + uniform(0.01,0.2), wsm);  //create variable to customize delay?
    scheduleAt(simTime() + 1, wsm);  //create variable to customize delay?

}

void TraCIDemo11p::onWSA(WaveServiceAdvertisment* wsa) {
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(wsa->getTargetChannel());
        currentSubscribedServiceId = wsa->getPsid();
        if  (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}

void TraCIDemo11p::onWSM(WaveShortMessage* wsm) {
    findHost()->getDisplayString().updateWith("r=16,green");
    if (mobility->getRoadId()[0] != ':') traciVehicle->changeRoute(wsm->getWsmData(), 9999);
    if (!sentMessage) {
        sentMessage = true;
        //repeat the received traffic update once in 2 seconds plus some random delay
        wsm->setSenderAddress(myId);
        wsm->setSerial(3);
        scheduleAt(simTime() + 2 + uniform(0.01,0.2), wsm->dup());
    }

}

void TraCIDemo11p::onCWSM(WaveShortMessage* wsm) {  //Updated sim code, custom method to handle receiving a "custom wave short message".  This only prints out the message when received at this time
    MessageList.push_back(wsm->getWsmData());
    aggregateMessage(wsm);
    //cout<<"Aggregated ----->"<<aggregatedMessage->getWsmData()<<endl;

    if (!sentMessage) {
            sentMessage = true;
            //repeat the received traffic update once in 2 seconds plus some random delay
            aggregatedMessage->setSenderAddress(myId);
            scheduleAt(simTime() + 2 + uniform(0.01,0.2), aggregatedMessage->dup());
        }
}

void TraCIDemo11p::handleSelfMsg(cMessage* msg) {

    if (WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg)) {
        sendDown(wsm->dup());
        wsm->setSerial(wsm->getSerial() +1);

        if (wsm->getWsmData() != NULL) {
            sendDown(wsm->dup());
            scheduleAt(simTime()+1, wsm);
        }
        //send this message on the service channel until the counter is 3 or higher.
        //this code only runs when channel switching is enabled
        else if (wsm->getSerial() >= 3) {
            //stop service advertisements
            stopService();
            delete(wsm);
        }
        else {
            scheduleAt(simTime()+1, wsm);
        }
    }
    else {
            BaseWaveApplLayer::handleSelfMsg(msg);
    }
}

void TraCIDemo11p::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //sends message every 1 seconds
        if (simTime() - lastDroveAt >= 1) {
            sendDown(aggregatedMessage);
            lastDroveAt = simTime();
        }
    // stopped for for at least 1s?
/*    if (mobility->getSpeed() < 1) {
        if (simTime() - lastDroveAt >= 10 && sentMessage == false) {
            findHost()->getDisplayString().updateWith("r=16,red");
            sentMessage = true;

            WaveShortMessage* wsm = new WaveShortMessage();
            populateWSM(wsm);
            wsm->setWsmData(mobility->getRoadId().c_str());

            //host is standing still due to crash
            if (dataOnSch) {
                startService(Channels::SCH2, 42, "Traffic Information Service");
                //started service and server advertising, schedule message to self to send later
                //scheduleAt(computeAsynchronousSendingTime(1,type_SCH),wsm);
                cout<<"in async"<<endl;
                scheduleAt(computeAsynchronousSendingTime(1,type_SCH),aggregatedMessage);
            }
            else {
                //send right away on CCH, because channel switching is disabled
                //sendDown(wsm);
                sendDown(aggregatedMessage);
            }
        }
    }
    else {
        lastDroveAt = simTime();
    }*/
}

void TraCIDemo11p::writeMessages(){  //anirban code
    BaseWaveApplLayer::writeMessages();
}

void TraCIDemo11p::aggregateMessage(WaveShortMessage* newmsg){
    aggregatedMessage = new WaveShortMessage(); //new aggregate message
    string dummyTag(numRSUs, '0');
    dummyTag+=":000";
    populateWSM(aggregatedMessage, 0, 0, dummyTag.c_str());
    aggregatedMessage->setPsid(-10);
    int n = MessageList.size();
    srand((int)time(0));
    int i = rand() % n +1;
    int Li = i;
    int j;
    while (i<Li+n){

        if(i==n){
            j = n;
        }
        else{
            j = i%n;
        }
        redundancyAvoidance(aggregatedMessage, MessageList.at(j-1));
        i++;
    }
}

void TraCIDemo11p::redundancyAvoidance(WaveShortMessage* aggmsg, string newmsg){
    vector<string> agg_wsmdata;
    string tempaggmsgData = aggmsg->getWsmData();
    boost::split(agg_wsmdata,tempaggmsgData, [](char c){return c== ':';});
    vector<string> new_wsmdata;
    boost::split(new_wsmdata, newmsg, [](char c){return c== ':';});

    bool flag = false;

    for(int i=0; i<numRSUs; i++){
        if(agg_wsmdata[0][i] == new_wsmdata[0][i]){
            if(agg_wsmdata[0][i] =='1'){
                flag=true;
            }
        }
    }
    if(flag ==false){ //only executes if there is nothing common between two messages
        string msgTagstr = "";
        for (int j=0; j< numRSUs; j++){
            int t1 = agg_wsmdata[0].at(j) - '0';
            int t2 = new_wsmdata[0].at(j) - '0';
            msgTagstr+= std::to_string(t1+t2);
        }
        int aggcont = stoi(new_wsmdata[1]) + stoi(agg_wsmdata[1]);
        msgTagstr+= ":"+ boost::lexical_cast<string>(aggcont);
        populateWSM(aggregatedMessage, 0, 0, msgTagstr.c_str());
        aggregatedMessage->setPsid(-10);
    }
}
