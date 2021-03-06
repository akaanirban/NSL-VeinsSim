// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
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

//package org.mixim.examples.ieee80211p;


import org.car2x.veins.base.modules.IMobility;
import org.car2x.veins.base.modules.IBaseApplLayer;
import org.car2x.veins.modules.nic.Nic80211p;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;
//import inet.mobility.static.StationaryMobility;

//
// Example host module using an 802.11p Network interface card.
//
module RSU
{
    parameters:
        //int xRange;
        //int yRange;
        string applicationType = default("BaseWaveApplLayer");
        string mobilityType = default("LinearMobility");
        bool initFromDisplayString = false;
        @display("p=2040,3000;i=device/antennatower;b=10,10,oval;r=90");

    gates:
        input radioIn; // gate for sendDirect

    submodules:
        mobility: <mobilityType> like IMobility {
        //mobility: StationaryMobility {
            parameters:
                //@display("p=2040,3000;i=block/cogwheel");
                //@display("p=1200,1300;i=veins/sign/yellowdiamond;is=vs");
        }
        
        nic: Nic80211p {
            parameters:
                @display("p=70,380;i=block/wrxtx,#008000");
        }
        appl: <applicationType> like IBaseApplLayer {
            parameters:
                @display("p=70,30;i=app");
        }

    connections allowunconnected:
        nic.upperLayerOut --> appl.lowerLayerIn;
        nic.upperLayerIn <-- appl.lowerLayerOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

        radioIn --> nic.radioIn;
}