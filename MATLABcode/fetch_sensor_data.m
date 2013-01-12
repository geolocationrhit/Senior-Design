% function [lat, lon, time, heading, RSS] = fetch_sensor_data(sensor_id)
function [dataStruct] = fetch_sensor_data(sensor_id)

t = tcpip(sensor_id,5005); % this was hardcoded before when everything worked....in case this quits working for some reason
                            % and we need to revert
fopen(t);

set(t,'Terminator','LF')

fprintf(t, sprintf('DataQuery for sensor %s', sensor_id));

%pause(1);

while(get(t,'BytesAvailable') == 0)
   % do nothing 
end

while (get(t,'BytesAvailable') > 0)
        t.BytesAvailable;
        dataReceived = fscanf(t);
end

%parsedData = sscanf(dataReceived,'LAT:%f LONG:%f TIME:%f HEADING:%f RSS:%f');
parsedData = regexp(dataReceived,'[-+]?[0-9]*\.?[0-9]+','match');
dataStruct = struct('lat', str2double(parsedData{1}), ...
    'long',str2double(parsedData{2}), ...
    'time',str2double(parsedData{3}), ...
    'heading',str2double(parsedData{4}), ...
    'RSS',str2double(parsedData{5}));

fclose(t);
delete(t);
clear t
    