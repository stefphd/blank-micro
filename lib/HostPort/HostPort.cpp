
#include "HostPort.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//Constructors
HostPort::HostPort(Stream* serial, uint32_t start_bytes, uint32_t stop_bytes) {
	init(); //general init
	_serial = serial;
	_header = start_bytes; //set the header bytes
	_terminator = stop_bytes; //set the terminator bytes
}

HostPort::HostPort(Stream* serial, uint32_t start_bytes) {
	init(); //general init
	_serial = serial;
	_header = start_bytes; //set the header bytes
}

HostPort::HostPort(Stream* serial) {
	init(); //general init
	_serial = serial;
}

// General initialization in constructors
void HostPort::init(void) {
	//reset vars
	_header = 0;
	_terminator = 0;
	_numObj_rx = 0;
	_numObj_tx = 0;
	_totSize_tx = 0;
}

//Attach functions
boolean HostPort::attachRx(uint8_t* pointer, size_t size) { //attach for rx
	if (_numObj_rx >= MAX_OBJS) { //max MAX_OBJS objects can be attached
		return false;
	}
	if ((_totSize_rx + size) > (BUF_SIZE - 8)) { //tot bytes to receive greater than BUF_SIZE (8 bytes are for header and terminator bytes)
		return false;
	}
	_ptr_rx[_numObj_rx] = pointer; //assign pointer
	_size_rx[_numObj_rx] = size; //assign size
	_numObj_rx++; //increase num of attached objects
	_totSize_rx += size; //increase tot rx bytes
	return true; //return true
}

boolean HostPort::attachTx(uint8_t* pointer, size_t size) { //attach for tx
	if (_numObj_tx >= MAX_OBJS) { //max MAX_OBJS objects can be attached
		return false;
	}
	if ((_totSize_tx + size) > (BUF_SIZE - 8)) { //tot bytes to send greater than BUF_SIZE (8 bytes are for header and terminator bytes)
		return false;
	}
	_ptr_tx[_numObj_tx] = pointer; //assign pointer
	_size_tx[_numObj_tx] = size; //assign size
	_numObj_tx++; //increase num of attached objects
	_totSize_tx += size; //increase tot tx bytes
	return true; //return true
}

//write data to serial
boolean HostPort::write() {
	if (!(_serial)) { //serial not available
		return false;
	}
	if (_numObj_tx == 0) { //nothing to transmit
		return false;
	}

	uint8_t c = 0; //index counter for buf (global counter)

	//put start bytes in buf if necessary
	//note that c++ return the current value of c, and then increment c, e.g. if c=4, _tx_buf[c++] returns _tx_buf[0] and puts c=1
	if (_header != 0) {
		_tx_buf[c++] = _header & MASK;
		_tx_buf[c++] = (_header >> 8) & MASK;
		_tx_buf[c++] = (_header >> 16) & MASK;
		_tx_buf[c++] = (_header >> 24) & MASK;
	}

	//put each byte of attached objects in buf
	for (uint8_t j = 0; j < _numObj_tx; j++) {
		for (uint8_t k = 0; k < _size_tx[j]; k++) _tx_buf[c++] = *(_ptr_tx[j] + k); //get the data stored in RAM address
	}

	//put stop bytes in buf if necessary
	if (_terminator != 0) {
		_tx_buf[c++] = _terminator & MASK;
		_tx_buf[c++] = (_terminator >> 8) & MASK;
		_tx_buf[c++] = (_terminator >> 16) & MASK;
		_tx_buf[c++] = (_terminator >> 24) & MASK;
	}

	//write buf to serial
	if (c > BUF_SIZE) { //too data to be sent
		return false;
	}
	_serial->write(_tx_buf, c); //write c elements of the trasmit array
	return true; //all is ok
}

//read data from serial
boolean HostPort::read() {
	if (!(_serial)) { //serial not available
		return false;
	}

	if (_numObj_rx == 0) { //nothing to receive
		return false;
	}

	if (!(_serial->available())) { //no available data
		return false;
	}

	uint8_t c = 0; //counter
	uint32_t time0 = micros(); //start time for timeout

	if (_header == 0) c = 4; //put i directly to 4 if no start bytes

	while ((micros() - time0) <= TIMEOUT_HOSTPORT) { //check timeout TIMEOUT_HOSTPORT
		if (_serial->available()) { //do stuff if serial data available 
			if (c < 4) { 
				if (_serial->read() == ((_header >> (8 * c)) & MASK)) { //read and compare to _header
					c++; //increment c if read is ok
					continue; //start new cycle as soon as data are available 
				}
			}
			else if (c == 4) { //read actual data
	
				/*for (uint8_t j = 0; j < _numObj_rx; j++) {
					if (_size_rx[j] != _serial->readBytes(_ptr_rx[j], _size_rx[j])) { //read for each attached rx objects and check size of data received
						return false; //return false if data size not consistent
					}
				}*/
				if (_totSize_rx != _serial->readBytes(_rx_buf, _totSize_rx)) { //read and save in buffer
							return false; //return false if data size not consistent
				}

				if (_terminator == 0) { //if _terminator==0 all is ok
					copyrx(); //copy buffer in rx objects
					return true; //all is ok
				}

				c++; //increment c when reading finished
				continue; //start new cycle
			}
			else { //c > 4, all data read 
				if (_serial->read() == ((_terminator >> (8 * (c - 5))) & MASK)) { //check stop bytes
					c++;  //increment c if read byte is ok
					if (c == 9) { //if c=9 all stop bytes read ok
						copyrx(); //copy buffer in rx objects
						return true; //all ok
					}
					else { 
						continue; //if c<9 (and c>4) start new cycle and read as soon as data available
					} 
				}
			}
			return false; //sth wrong
		}
	}
	return false; //sth wrong
}

void HostPort::copyrx() {
	uint8_t k = 0;
	for (uint8_t j = 0; j < _numObj_rx; j++) {
		memcpy(_ptr_rx[j], ((uint8_t*)_rx_buf + k), _size_rx[j]);
		k += _size_rx[j];
	}
}