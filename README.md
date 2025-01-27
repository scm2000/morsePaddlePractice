This project makes a Morse Code practice oscillator out of an arduino compatible micro controller,
a stero jack and a buzzer. The code, as checked in, is targeted to the Teensy 4.1 controller.
The code is set up for a 3 pin active buzzer that is active low, simple changes can be made for active high buzzers,
and 3 pin passive buzzers can also be used.

With simple wiring of the parts (in comments in main.cpp), and some simple software
it impelemnts an Iambic-A code practice oscillator.  

Upon startup the user can practice sending morse characters, the code will display
the characters received.   The code enforces the standard Morse timings for
inter-letter and inter-word timings.

There is an option to send 7 dots together to go into word practice mode.
In that case the code will play out the morse code for a random word out of a 
list of the 100 most common words in English.   The use then can use
this to practice copying, and then sending the word back. 
Should the user send back the correct word the code sends an R back and
sends out a new word.   Otherwise the code will resend the word.

The user can switch back to letter practice mode by sending 7 dahses together.

The user has the option of practicing soley by hearing, or can look at the 
console output to see the word being sent as a hint.
