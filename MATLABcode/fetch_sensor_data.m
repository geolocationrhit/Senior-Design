% function [lat, lon, time, heading, RSS] = fetch_sensor_data(sensor_id)
function [dataStruct] = fetch_sensor_data(sensor_id)

t = tcpip('127.0.0.1',5006);

fopen(t);

fprintf(t, sprintf('DataQuery for sensor %d', sensor_id));

pause(1);

set(t,'Terminator','LF')

while (get(t,'BytesAvailable') > 0)
        t.BytesAvailable
        dataReceived = fscanf(t)
end

parsedData = sscanf(dataReceived,'LAT:%f LONG:%f TIME:%d HEADING:%f RSS:%f');
dataStruct = struct('lat', parsedData(1),'long',parsedData(2),'time',parsedData(3),'heading',parsedData(4),'RSS',parsedData(5))

fclose(t);
delete(t);
clear t
    