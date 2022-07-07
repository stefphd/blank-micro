/*! \file main.cpp
	\brief Main file of the low-level code for the controller.
	\details Main file of the low-level code for the controller. This contains the definition for main function, which is the
	entry-point function of the code. Note that this is different from the standard Arduino workflow, which makes use of the setup() and
	loop() function with the main() written in the core code (actually, the main function defined in this file overrides that
	in the core code).
	\see mainfun
*/

//include
#include <Arduino.h>
#include <libs.h>

#undef main //remove Arduino main function - maybe not necessary (?)

/*! @defgroup mainfun Entry-point function
	\brief Entry-point function of the code.
	\details This contains the definition for main function, which is the
	entry-point function of the code. Note that this is different from the standard Arduino workflow, which makes use of the setup() and
	loop() function with the main() written in the core code (actually, the main function defined in this file overrides that
	in the core code).
    \see main.cpp
    @{
*/

/*! \brief Entry-point function.
	\details Definition for main function, which is the entry-point function of the code.
	The function implementation is structured as follows

	```
	int main() {

		//initializations here

		while(1) { //infinite loop, running forever
			//loop here
		}

		return 0;
	}
	```

	\return The exit status (0 by default).
    \see main.cpp
*/
int main() {

	//initializations here

	while(1) { //infinite loop, running forever
		//loop stuff here
	}

	return 0; //mandatory
}

/*! @} */