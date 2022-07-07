
#ifndef _HOSTPORT_H
#define _HOSTPORT_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*! \brief A class for communication with PC via USB/serial.
	\details The class manages the communication between a microcontroller and 
	a host PC via USB/serial. The trasmitted or received data packets consists of:
	- A possible 4-bytes header (disabled with HostPort::NULL_HEADER).
	- The data bytes, for a maximum of BUF_SIZE bytes with MAX_OBJS objects.
	- A possible 4-bytes terminator (disabled with HostPort::NULL_TERMINATOR).

	The HostPort object is created using

	```c++
	HostPort hostPort(&Serial, HostPort::NULL_HEADER, HostPort::NULL_TERMINATOR); //with neither header nor terminator
	HostPort hostPort(&Serial); //with neither header nor terminator
	HostPort hostPort(&Serial, header, HostPort::NULL_TERMINATOR); //with header only
	HostPort hostPort(&Serial, header); //with header only
	HostPort hostPort(&Serial, HostPort::NULL_HEADER, terminator); //with terminator only
	HostPort hostPort(&Serial, header, terminator); //with header and terminator
	```

	Objects can be attached to the trasmit buffer using (for a maximum of MAX_OBJS objects)
	
	```c++
	hostPort.attachTx((uint8_t*) &object1, sizeof(object1));
	hostPort.attachTx((uint8_t*) &object2, sizeof(object2));
	```

	Objects can be attached to the receive buffer using (for a maximum of MAX_OBJS objects)
	
	```c++
	hostPort.attachRx((uint8_t*) &object3, sizeof(object3));
	hostPort.attachRx((uint8_t*) &object4, sizeof(object4));
	```

	Finally trasfering and receiving is performed using

	```c++
	hostPort.write();
	hostPort.read();
	```
	
	\author Stefano Lovato
	\date 2022
*/

//using constexpr instead
//#define BUF_SIZE 256 //size of buffers
//#define MAX_OBJS 3 //max num of attached objects
//#define MASK 0x000000FF //mask for bitwise operations
//#define TIMEOUT_HOSTPORT 500 //timeout (us)

class HostPort {
public:
	//constructors
	/*! \brief Contructor with header and terminator.
		\details The constructor with user-defined header and terminator.
		\param serial The pointer to the Stream object used for the serial.
		\param header The 4-bytes header of the data packet. Use HostPort::NULL_HEADER for no header.
		\param terminator The 4-bytes terminator of the data packet. Use HostPort::NULL_TERMINATOR for no header.
	*/
	HostPort(Stream* serial, uint32_t header, uint32_t terminator); //constructor with both header and terminator

	/*! \brief Contructor with header only.
		\details The constructor with user-defined header and no terminator.
		\param serial The pointer to the Stream object used for the serial.
		\param header The 4-bytes header of the data packet. Use HostPort::NULL_HEADER for no header.
	*/
	HostPort(Stream* serial, uint32_t header); //constructor with header only

	/*! \brief Contructor with no header and terminator.
		\details The constructor with no header and terminator.
		\param serial The pointer to the Stream object used for the serial.
	*/
	HostPort(Stream* serial); //constructor w/o header and terminator

	//funs
	/*! \brief Attach object for receiving.
		\details The function attaches an object to the receive buffer. 
		A number of MAX_OBJS objects can be attached, for a total of maximum BUF_SIZE bytes.
		\param pointer The pointer to the object to attach. Cast to uint8_t is required.
		\param size Thesize to the object to attach.
		\return True if success, false if the buffer size or the number of objects exceeds BUF_SIZE or MAX_OBJS.
		\see MAX_OBJS BUF_SIZE
	*/
	boolean attachRx(uint8_t* pointer, size_t size); //attach object for rx

	/*! \brief Attach object for tranmission.
		\details The function attaches an object to the trasmit buffer. 
		A number of MAX_OBJS objects can be attached, for a total of maximum BUF_SIZE bytes.
		\param pointer The pointer to the object to attach. Cast to uint8_t is required.
		\param size Thesize to the object to attach.
		\return True if success, false if the buffer size or the number of objects exceeds BUF_SIZE or MAX_OBJS.
		\see MAX_OBJS BUF_SIZE
	*/
	boolean attachTx(uint8_t* pointer, size_t size); //attach object for tx

	/*! \brief Write to serial.
		\details The function writes the attached transmit objects to the serial.
		\return True if success.
		\attention The serial object must be started by the user before writing.
	*/
	boolean write(); //write attached objects, return true if data written

	/*! \brief Read from serial.
		\details The function reads a data packet from the serial and saves it in the attached receive objects.
		\return True if success.
		\attention The serial object must be started by the user before reading.
	*/
	boolean read(); //read attached object, return true is data read

	//static constexpr
	static constexpr uint32_t NULL_HEADER = 0x00000000; //!< Null header. \details The value used for no header.
	static constexpr uint32_t NULL_TERMINATOR = 0x00000000; //!< Null terminator. \details The value used for no terminator.

private:

	//static constexpr
	static constexpr size_t BUF_SIZE = 256; //!< Buffer size. \details The size of the transmit and receive buffers.
	static constexpr size_t MAX_OBJS = 4; //!< Maximum objects. \details The number of maximum attached objects to the transmit and receive buffers-
	static constexpr uint32_t MASK = 0x000000FF; //!< A mask for parsing stuff.
	static constexpr uint32_t TIMEOUT_HOSTPORT = 500; //!< Timeout for the reading (us).

	//funs
	/*! \brief Initialize the host port.
		\details The function performs the inizializaton of the host port.
	*/
	void init(void); //general init in costructors

	/*! \brief Copy receive buffer into receive objects
		\details The function copies the receive buffer into the receive attached objects when the read is successful.
	*/
	void copyrx(void); //copy _rx_buf in _ptr_rx when reading was ok

	//vars
	uint8_t _rx_buf[BUF_SIZE] = { 0 }; //!< Receive buffer. \details The receive buffer has size BUF_SIZE. \see BUF_SIZE attachRx
	uint8_t _tx_buf[BUF_SIZE] = { 0 }; //!< Transmit buffer. \details The transmit buffer has size BUF_SIZE. \see BUF_SIZE attachTx
	uint32_t _header = NULL_HEADER; //!< 4-bytes header. \details The 4-bytes header is null when equal to NULL_HEADER. \see NULL_HEADER
	uint32_t _terminator = NULL_TERMINATOR; //!< 4-bytes terminator. \details The 4-bytes terminator is null when equal to NULL_TERMINATOR. \see NULL_TERMINATOR
	uint8_t* _ptr_rx[MAX_OBJS] = { nullptr }; //!< Pointers to receive objects. \details The arrays of pointers to the receive attached objects. It has size MAX_OBJS. \see MAX_OBJS attachRx
	uint8_t* _ptr_tx[MAX_OBJS] = { nullptr }; //!< Pointers to transmit objects. \details The arrays of pointers to the transmit attached objects. It has size MAX_OBJS. \see MAX_OBJS attachTx
	size_t _size_rx[MAX_OBJS] = { 0 }; //!< Sizes of receive objects. \details The arrays of sizes of the receive attached objects. It has size MAX_OBJS. \see MAX_OBJS attachRx
	size_t _size_tx[MAX_OBJS] = { 0 }; //!< Sizes of transmit objects. \details The arrays of sizes of the transmit attached objects. It has size MAX_OBJS. \see MAX_OBJS attachTx
	size_t _totSize_rx = 0; //!< Total size of receive objects. \details The total size of receive objects must be lower than BUF_SIZE. \see BUF_SIZE
	size_t _totSize_tx = 0; //!< Total size of transmit objects. \details The total size of transmit objects must be lower than BUF_SIZE. \see BUF_SIZE
	uint8_t _numObj_rx = 0; //!< Total number of receive objects. \details The total number of receive objects must be lower than MAX_OBJS. \see MAX_OBJS
	uint8_t _numObj_tx = 0; //!< Total number of transmit objects. \details The total number of transmit objects must be lower than MAX_OBJS. \see MAX_OBJS
	Stream* _serial = nullptr; //!< Pointer to Stream object. \details The pointer to a Stream object representing the serial. \attention The serial must be started before using this class.
};

#endif