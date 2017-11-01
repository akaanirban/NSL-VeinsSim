### This is the simulation config files for Veins framework on OMNET++ 4.6

### To run :
Firstly copy `/examples/anirban_veins` inside `/veins-veins-4.6/examples` folder. This is a new run scenario. The network file is (stupidly) named `test1.ned` and it has config to generate multiple RSUs. Rest of the files are pretty self explanatory. 

Then copy the `src/veins/modules/application` folder and replace the generic folder in the veins installation with the same name.

### Folder Structure:
```
.
+-- examples
|   +-- anirban_veins (changes done here)
|   +-- veins
+-- src
|   +-- scripts
|   +-- veins
	|   +-- base
	|   +-- nodes
	|   +-- modules
		|   +-- analogueModel
		|   +-- application
			|   +-- ieee80211p 
				|   +-- BaseWaveApplLayer.cc (changes done here)
				|   +-- BaseWaveApplLayer.h (changes done here)
				|   +-- BaseWaveApplLayer.ned (changes done here)
			|   +-- traci (changes done here)
				|   +-- TraCIDemo11p.cc (changes done here)
				|   +-- TraCIDemo11p.h (changes done here)
				|   +-- TraCIDemo11p.ned (changes done here)
				|   +-- TraCIDemoRSU11p.cc (changes done here)
				|   +-- TraCIDemoRSU11p.h (changes done here)
				|   +-- TraCIDemoRSU11p.ned (changes done here)
		|   +-- mac
		|   +-- messages
		|   +-- mobility
		|   +-- nic
		|   +-- obstacle
		|   +-- phy
		|   +-- utility
		|   +-- world
+-- README.md
```
### TODO:
1. Right now , generating n random RSU a hotspot by generating n random numbers in python. 
2. Need to make this automatic somehow.
3. Need to make sure of the transmitting pattern of the nodes. Right now they are voluntarily broadcasting every second.
4. Need to adjust the transmission power.
