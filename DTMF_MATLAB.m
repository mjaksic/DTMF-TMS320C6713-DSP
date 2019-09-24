clc;
close all;
clear all;

%Definiranje matrice 4x4 za DTMF ton i frekvencija 
keypad = ['1', '2', '3', 'A'; '4', '5', '6', 'B'; '7', '8', '9', 'C'; '*', '0', '#', 'D'];
fLow = [697 770 852 941];
fHigh = [1209 1336 1477 1633];

%Broj uzoraka i generiranje matrica za realni/imaginarni dio, magnitudu i podatak o DTMF tonu
N = 256;
Fs = 8000;
Xre = zeros(1,N);
Xim = zeros(1,N);
mag_data = zeros(1,N);
data = zeros(Fs, 1);

seconds = 1; %trajanje DTMF tona

%Generiranje polja t od 1/8000 do 1
t = 1:Fs;
t = t/Fs;

%?ekanje na znak s tipkovnice
znak = input('Unesite znak: ','s');

%Odredivanje frekvencija za uneseni znak
if(znak == '1')
    lowFreq = 697;
    highFreq = 1209;
elseif(znak == '2')
    lowFreq = 697;
    highFreq = 1336;
elseif(znak == '3')
    lowFreq = 697;
    highFreq = 1477;
elseif(znak == 'A' || znak == 'a')
    lowFreq = 697;
    highFreq = 1633;
elseif(znak == '4')
    lowFreq = 770;
    highFreq = 1209;
elseif(znak == '5')
    lowFreq = 770;
    highFreq = 1336;
elseif(znak == '6')
    lowFreq = 770;
    highFreq = 1477;
elseif(znak == 'B' || znak == 'b')
    lowFreq = 770;
    highFreq = 1633;
elseif(znak == '7')
    lowFreq = 852;
    highFreq = 1209;
elseif(znak == '8')
    lowFreq = 852;
    highFreq = 1336;
elseif(znak == '9')
    lowFreq = 852;
    highFreq = 1477;
elseif(znak == 'C' || znak == 'c')
    lowFreq = 852;
    highFreq = 1633;
elseif(znak == '*')
    lowFreq = 941;
    highFreq = 1209;
elseif(znak == '0')
    lowFreq = 941;
    highFreq = 1336;
elseif(znak == '#')
    lowFreq = 941;
    highFreq = 1477;
elseif(znak == 'D' || znak == 'd')
    lowFreq = 941;
    highFreq = 1633;
else fprintf('Unesen krivi broj!');
end

%Generiranje DTMF tona
for n = 1:Fs
data(n) = sin(2*pi*lowFreq*t(n)) + sin(2*pi*highFreq*t(n));
end

data = repmat(data,seconds,1);
soundsc(data);

%Fourierova transformacija i jacina spektra
 
 for k = 0:1:N-1
     Xre(k+1) = 0;
     for n = 0:1:N-1
         Xre(k+1) = Xre(k+1) + data(n+1) * cos((n) * (k) * 2 * pi / (N));
     end
     Xim(k+1) = 0;
     for n = 0:1:N-1
         Xim(k+1) = Xim(k+1) + data(n+1) * sin((n) * (k) * 2 * pi / (N));
     end
     mag_data(k+1) = sqrt(Xre(k+1) * Xre(k+1) + Xim(k+1) * Xim(k+1));
 end
 
 DTMF697 = round((N/Fs)*697) + 1; 
 DTMF770 = round((N/Fs)*770) + 1;
 DTMF852 = round((N/Fs)*852) + 1;
 DTMF941 = round((N/Fs)*941) + 1;
 DTMF1209 = round((N/Fs)*1209) + 1;
 DTMF1336 = round((N/Fs)*1336) + 1;
 DTMF1477 = round((N/Fs)*1477) + 1;
 DTMF1633 = round((N/Fs)*1633) + 1;
  
 
%Niska frekvencija

if(mag_data(DTMF697) > mag_data(DTMF770) && mag_data(DTMF697) > mag_data(DTMF852) && mag_data(DTMF697) > mag_data(DTMF941))
    lowIndex = 1;
end
if(mag_data(DTMF770) > mag_data(DTMF697) && mag_data(DTMF770) > mag_data(DTMF852) && mag_data(DTMF770) > mag_data(DTMF941))
    lowIndex = 2;
end
if(mag_data(DTMF852) > mag_data(DTMF697) && mag_data(DTMF852) > mag_data(DTMF770) && mag_data(DTMF852) > mag_data(DTMF941))
    lowIndex = 3;
end
if(mag_data(DTMF941) > mag_data(DTMF697) && mag_data(DTMF941) > mag_data(DTMF770) && mag_data(DTMF941) > mag_data(DTMF852))
    lowIndex = 4;
end


%Visoka frekvencija
if(mag_data(DTMF1209) > mag_data(DTMF1336) && mag_data(DTMF1209) > mag_data(DTMF1477) && mag_data(DTMF1209) > mag_data(DTMF1633))
    highIndex = 1;
end
if(mag_data(DTMF1336) > mag_data(DTMF1209) && mag_data(DTMF1336) > mag_data(DTMF1477) && mag_data(DTMF1336) > mag_data(DTMF1633))
    highIndex = 2;
end
if(mag_data(DTMF1477) > mag_data(DTMF1209) && mag_data(DTMF1477) > mag_data(DTMF1336) && mag_data(DTMF1477) > mag_data(DTMF1633))
    highIndex = 3;
end
if(mag_data(DTMF1633) > mag_data(DTMF1209) && mag_data(DTMF1633) > mag_data(DTMF1336) && mag_data(DTMF1633) > mag_data(DTMF1477))
    highIndex = 4;
end
 
fprintf('\nDETEKCIJA\nZnak: %s\nFrekvencija znaka: Low: %d, High: %d\n',keypad(lowIndex,highIndex),fLow(lowIndex),fHigh(highIndex));