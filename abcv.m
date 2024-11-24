% Parametrii sistemului
h = 0.1; % Timpul de eșantionare (100 ms)
Kp = 2.3;   % Coeficient proporțional
Ki = 2.2;   % Coeficient integrator
Kd = 0.002;   % Coeficient derivativ
T = 1;      % Constanta de timp pentru bec

% Calculul coeficienților pentru PID discret
c1 = 100 * Kd;
c2 = -Kp - 200 * Kd;
c3 = 100 * Kd + Kp + 0.01 * Ki;

% Funcția de transfer a PID-ului discret
num_PID = [c3, c2, c1];  % Coeficienții numeratori
den_PID = [1, -1, 0];    % Coeficienții numitorului
PID_discret = tf(num_PID, den_PID, h);

% Coeficienții pentru PT1 discret
a1 = h/(h+T);
b = T/(h+T);

% Funcția de transfer a PT1 discret
num_PT1 = [a1, 0];       % Coeficienții numeratori
den_PT1 = [1, -b];       % Coeficienții numitorului
PT1_discret = tf(num_PT1, den_PT1, h);

% Parametrii temperaturii țintă
Temp_initial = 25;       % Temperatura inițială (°C)
Temp_final = 50;        % Temperatura finală dorită (°C)
Timp_incalzire = 60;     % Timpul în care să ajungă la Temp_final (s)
pas_crestere = (Temp_final - Temp_initial) / Timp_incalzire;

% Generarea temperaturii țintă (Temp_target)
time = 0:h:100; % Timp de simulare (100 secunde)
Temp_target = Temp_initial + min(time, Timp_incalzire) * pas_crestere;

% Setpoint (temperatura dorită)
T_set = Temp_final;

% Crearea sistemului în buclă închisă
closed_loop_system = feedback(PID_discret * PT1_discret, 1);

% Inițializarea temperaturii efective
initial_temp = Temp_initial;

% Simularea răspunsului sistemului
[response, t] = lsim(closed_loop_system, Temp_target, time);

% Graficul temperaturilor
figure;
plot(t, Temp_target, 'r-', 'LineWidth', 1.5); % Temperatura țintă (linie continuă)
hold on;
yline(T_set, 'r--', 'LineWidth', 1.5);         % Setpoint-ul (linie punctată)
plot(t, response + initial_temp - response(1), 'b', 'LineWidth', 1.5); % Temperatura efectivă
grid on;
title('Comparație între temperatura țintă și răspunsul sistemului');
xlabel('Timp (s)');
ylabel('Temperatura (\circC)');
legend('Temperatura target', 'Temperatura dorită (T\_set)', 'Răspunsul sistemului');
