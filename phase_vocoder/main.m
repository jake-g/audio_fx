% Input
input = 'speech_files/Maple.wav' % piano 
% input = 'speech_files/vowels_10Hz_edited.wav'
% input = 'speech_files/vowels_300Hz_edited.wav'

speed_factor = 0.3;  % slow down
fft_size = 1024;

% Change Speed
[x,sr] = audioread(input);
y = pvoc(x, speed_factor); 
len = max(length(y), length(x)); % longer signal

%% Play 
sound(x, sr) 
pause(); % press any key
sound(y, sr)

% Plot
figure
subplot(2,1,1); plot(x); xlim([0, len]); title('Original')
subplot(2,1,2); plot(y); xlim([0, len]); title('Stretched')