//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//import org.car2x.veins.nodes.RSU;
import RSU;
import org.car2x.veins.nodes.Scenario;

//network test1 extends Scenario


network test1 extends Scenario
{	
    parameters:
        int totalRSUs;
    	@display("bgb=2500,2500");
    submodules:
        rsu[totalRSUs]: RSU {
            //@display("p=1500,1240");

			mobility.x = -1; //-1 for random placement
			mobility.y = -1;//-1 for random placement
			mobility.z = -1;//-1 for random placement
			
			//mobility.x = uniform(4000, 4500);
			//mobility.y = uniform(20, 100);
			//mobility.z = 0;
			
			mobility.initFromDisplayString = false;
			mobility.initialX = uniform(500, 4500);
			mobility.initialY = uniform(500, 4500);
			mobility.initialZ = 0;
			mobility.speed = 0;
			mobility.angle = 0;
			mobility.acceleration = 0;
			mobility.updateInterval =0;
        }
    connections allowunconnected:
}
