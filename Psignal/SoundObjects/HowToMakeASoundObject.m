% How to make a new sound object
% 
% The parent of all sounds is "SoundObject" class. All the userdefined classes 
% inherit fields and methods from this class. 
%
% The fields of SoundObject and their default values are:
% 	descriptor = none           % a text describing the class
% 	SamplingRate = 40000        % SamplingRate of the sound. 
% 	Loudness = 0                % Loudness. 0 means use the setting that
%                               %   user enters. but if you specify other
%                               %   than 0 it will be used as loudness
% 	PreStimSilence = 0.4        % Silence Gap before the stim starts
% 	PostStimSilence = 0.4       % Silence gap after the sound stops
% 	Names = 1x1 cell            % names of the sounds. Used for loading
%                               % them (if applicaple) and writing to event file.
% 	MaxIndex = 1                % Specify the maximum index of the sound,
%                               % oR in other word specifies how many different sounds this object
%                               % contains. 
% 	UserDefinableFields = 1x9 cell  % any item you put here will be shown
%       on the gui and the user can change their values during experiment setup. 
%       Each field should have a field name, style on the gui and a default value. 
%           (e.g.1  {'Duration, 'edit',1} ) 
%           (e.g.2  {'Frequency', 'popupmenu', '100|200|300'} )
% % ...
%     ,//
% 
% Event structure:
% you need to return the stimulus events in the following format:
% event is a structure array with the following fields:
%     Note: description (text). % Important: The actual sound should have 
%           STIM in the begining, seperated by a comma from the rest.
%           Look at the example.
%     StartTime: start of this event from the begining of the sound
%     StopTime: stop of this event from the begining of the sound
%
% Example:
% event(1).Note = 'PreStimSilence, Torc2';
% event(1).StartTime = 0;
% event(1).StopTime = .4;
% event(2).Note = 'Stim, Torc2';
% event(2).StartTime = 0.4;
% event(2).StopTime = 3.4;
% event(3).Note = 'PostStimSilence, Torc2';
% event(3).StartTime = 3.4;
% event(3).StopTime = 3.8;
   

% To add your object, just copy the directory to Baphy/SoundObjects. Copy
% get.m, set.m and ObjUpdate.m from any other object (they are all the same) and you are
% done.