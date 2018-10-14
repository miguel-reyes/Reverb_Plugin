# Reverb_Plugin
Repository for the reverb plugin developed for Pd as part of the class "Strukturiertes Programmieren in C++" at the TU Berlin


Part of the Curriculum for the Master in Science Program "Audiokommunikation und -technologie"

Professor Name: Thomas Resch

Student Names: Marquis Fields, Miguel Reyes Botello and Malte Schneider

Included files are the C programs for the all_pass and comb_pass filters that are meant to be used by the program mf_reverb_pd and create a reverb effect with them. 

A debug pd patch is included to test the reverb by loading a wav file to pd. The file is meant to be loaded using the openpanel button and played by pressing on the message box for readsf~. 
There is also a wet value slider to adjust how much of the processed signal is added to the original. 

The pd object allows changing the value in ms for the reverb, which can be directly typed into the object after the tilde, actual value for the uploaded file is 3s. Additionally, in case that the reverb starts acting in an unexpected way, a panic button is included to mute the output of the object to dac~.

An audio snippet of a piano is attached to test the Pd object.
