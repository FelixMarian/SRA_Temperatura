% Parametrii sistemului
h = 0.1; % Timpul de eșantionare (100 ms)
Kp = 1;   % Coeficient proporțional
Ki =2; % Coeficient integrator
Kd = 0; % Coeficient derivativ
T = 1; % Constanta de timp pt becul nostru

% Calculul coeficienților pentru PID discret
c1 = 100 * Kd;
c2 = -Kp - 200 * Kd;
c3 = 100 * Kd + Kp + 0.01 * Ki;

% Funcția de transfer a PID-ului discret
num_PID = [c3, c2, c1];  % Coeficienții numeratori
den_PID = [1, -1, 0];    % Coeficienții numitorului

% Definirea funcției de transfer discrete pentru PID
PID_discret = tf(num_PID, den_PID, h);

% Coeficienții pentru PT1 discret
a1 = h/(h+T);
b = T/(h+T);

% Funcția de transfer a PT1 discret
num_PT1 = [a1, 0];       % Coeficienții numeratori
den_PT1 = [1, -b];       % Coeficienții numitorului

% Definirea funcției de transfer discrete pentru PT1
PT1_discret = tf(num_PT1, den_PT1, h);

% Setpoint-ul (valoarea dorită a temperaturii)
setpoint = 50;

% Salvează parametrii în workspace-ul MATLAB
disp('Parametrii calculați:');
disp(['c1 = ', num2str(c1)]);
disp(['c2 = ', num2str(c2)]);
disp(['c3 = ', num2str(c3)]);
disp(['a1 = ', num2str(a1)]);
disp(['b = ', num2str(b)]);

% Crearea sistemului în buclă închisă
% Feedback negativ pentru buclă PID -> PT1
closed_loop_system = feedback(PID_discret * PT1_discret, 1);

% Simularea răspunsului la un setpoint de 36.6 °C
time = 0:h:100; % Simulare pentru 10 secunde
input_signal = setpoint * ones(size(time)); % Setpoint constant

% Simulare
[response, t] = lsim(closed_loop_system, input_signal, time);

% Graficul răspunsului sistemului
figure;
plot(t, response, 'b', 'LineWidth', 1.5);
hold on;
yline(setpoint, 'r--', 'LineWidth', 1.5); % Linia de referință
grid on;
title('Răspunsul sistemului în buclă închisă');
xlabel('Timp (s)');
ylabel('Temperatura (\circC)');
legend('Răspunsul sistemului', 'Setpoint');
