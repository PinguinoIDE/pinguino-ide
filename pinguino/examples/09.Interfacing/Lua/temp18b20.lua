#!/usr/bin/lua

-- Read temperature from Pinguino connected with 18b20 sensor
-- Régis Blanchot 14/09/2010

require('libusb')

--
-- global variables
--

VENDOR = 0x04D8
PRODUCT = 0xFEAA
CONFIGURATION = 3
INTERFACE = 0
EP_IN = 0x82
EP_OUT = 0x01

--
-- read keyboard function in non blocking mode
--

function read_keyboard()
	charactere=io.popen("sh testkbd.sh","r")	-- open pipe and run command
	valeur=charactere:read()						-- read output of command
	charactere:close()								-- close pipe
	if valeur~=nil then
		return valeur
	end
end

--
-- try to find Pinguino
--

local buses=libusb.get_busses()    -- locate USB buses on the computer

function find_pinguino()
	for dirname, bus in pairs(buses) do
		local devices=libusb.get_devices(bus)
		for filename, device in pairs(devices) do
			local descriptor=libusb.device_descriptor(device)
			if ((descriptor.idVendor==VENDOR) and (descriptor.idProduct==PRODUCT)) then
				local handle=libusb.open(device)
				if ((libusb.set_configuration(handle, CONFIGURATION)) and (libusb.claim_interface(handle, INTERFACE))) then
					return handle
				else return nil
				end   
			end
		end
	end
end

--
-- main
--

pinguino=find_pinguino()	-- try to find a Pinguino
if (pinguino~=nil) then
	print('Pinguino initialised with success !!')
	print("Press a key to exit")
	while(read_keyboard()==nil) do
		-- int libusb.bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
		-- libusb.bulk_read performs a bulk read request to the endpoint specified by ep.
		-- returns number of bytes read on success or < 0 on error.
		libusb.bulk_read(pinguino, EP_IN, received_data, 4, 5000)
		if(received_data~=nil) then
			s = received_data[0]										-- read sign (1 byte)
			i = received_data[1]										-- read integer part (1 byte)
			f = received_data[2] * 256 + received_data[3]	-- read fractional part (2 bytes)
			--print(tonumber(received_data))
			print(string.format("Temperature is %d%d.%d°C ", s, i, f))
		end
	end
	libusb.close(pinguino)	-- close Pinguino
else
	print('Pinguino initialisation failed !!')
	os.exit()
end

