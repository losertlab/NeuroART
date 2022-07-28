%0 hz, 10 ms, 0 V (in) --> ~0mW for gated signal
[Data_0Hz_10ms_0Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

%0 hz, 10 ms, 0.5 V (in) --> ~8.5 mW for gated signal
[Data_0Hz_10ms_05Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

%0 hz, 10 ms, 1 V (in) --> ~28 mW for gated signal
[Data_0Hz_10ms_1Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

%0 hz, 10 ms, 2 V (in) --> ~52 mW for gated signal
[Data_0Hz_10ms_2Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

%5 hz, 10 ms, 1 V (in) --> 2 mW 
[Data_5Hz_10ms_1Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

%10 hz, 10 ms, 1 V (in) --> 2.7 mW 
[Data_10Hz_10ms_1Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

%20 hz, 10 ms, 1 V (in) --> 5.5 mW 
[Data_20Hz_10ms_1Vin names] = OptoTrigTest(10, 'Dev1', 'Differential');

Data=[];

Data.Opto_0Hz_10ms_0Vin.volts = Data_0Hz_10ms_0Vin;
Data.Opto_0Hz_10ms_0Vin.mW = 0;

Data.Opto_0Hz_10ms_05Vin.volts = Data_0Hz_10ms_05Vin;
Data.Opto_0Hz_10ms_05Vin.mW = 8.5;

Data.Opto_0Hz_10ms_1Vin.volts = Data_0Hz_10ms_1Vin;
Data.Opto_0Hz_10ms_1Vin.mW = 28;

Data.Opto_0Hz_10ms_2Vin.volts = Data_0Hz_10ms_2Vin;
Data.Opto_0Hz_10ms_2Vin.mW = 52;

Data.Opto_5Hz_10ms_1Vin.volts = Data_5Hz_10ms_1Vin;
Data.Opto_5Hz_10ms_1Vin.mW = 2;

Data.Opto_10Hz_10ms_1Vin.volts = Data_10Hz_10ms_1Vin;
Data.Opto_10Hz_10ms_1Vin.mW = 2.7;

Data.Opto_20Hz_10ms_1Vin.volts = Data_20Hz_10ms_1Vin;
Data.Opto_20Hz_10ms_1Vin.mW = 5.5;

Data.names = names;
Data.SteppedVolts = [0:.1:2; .2 .3 1.2 2.8 4.8 7.6 11.1 14.9 20 24 28.1 33.3 38.8 42 48 50.1 52.1 55.8 56.1 58 58]
save V:\Data\Opto\Calibration Data