**Metasounds Audio Math Utils**

This repo implements a collection of helpful audio processing functionality that I as a Max/Pd dev regularly use into Metasounds.

The objects are:
Wrap\~ = Wrap (Audio)  
Pow\~  = Pow (Audio)  
Sqrt\~ = Sqrt (Audio)  
Gate\~ = Gate (Audio)  
sin~ / cos~ = Sine / Cos (Phasor driven for precise phase accuracy)
VCF = 'Voltage Controlled Filter' SVF algorithm that can work with sample rate frequency and Q settings. This uses a TAN approximation under the surface for setting the coefs at sample rate but is still much heavier CPU-wise than the usual SVF, use at your own risk.

Other objects that differ from Pd:
Compare: an audio rate comparator object for doing ==, != etc at audio rate
Timer: Outputs the time between triggers
InterpTo: Audio rate interpolation object
Click: outputs a 1 sample click 'unit impulse'
AudioDivide: audio rate / . Divide by zero is always zero.

Note this repo contains no binaries, Win64 binary downloads can be found here: https://drive.google.com/drive/folders/1TJ3VeyfMrBBq0pOH1HmXQ_c73ePaErq9?usp=drive_link
If you need platforms other than windows you'll need to edit the cs plugin config files for compliation.

![CustomMetasoundObjects](https://github.com/Chris-TopherW/MetasoundsAudioMathUtils/assets/11866314/3c56d392-a4f5-4246-ab35-d858daf23569)
