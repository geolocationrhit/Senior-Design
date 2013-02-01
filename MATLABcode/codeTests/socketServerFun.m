% [lat, long, time, heading, RSS] = fetch_sensor_data(sensor_id)

t = tcpip('127.0.0.1',5005,'NetworkRole','server');

fopen(t);
fprintf(t, 'DataQuery');
%fprintf(t, 'DataQuery');

%pause(1);

%dataReceived = fscanf(t)
set(t,'Terminator','LF')
while(t.bytesAvailable == 0 )
    
end

dataReceived = fread(t,t.bytesAvailable,'char')
print dataReceived

%set(t,'Terminator','LF')
%{
while (get(t,'BytesAvailable') > 0)
        t.BytesAvailable
        dataReceived = fscanf(t)
        print dataReceived
end
%}
%parsedData = sscanf(dataReceived,'LAT:%f LONG:%f TIME:%d HEADING:%f RSS:%f')
%dataStruct = struct('lat', parsedData(1),'long',parsedData(2),'time',parsedData(3),'heading',parsedData(4),'RSS',parsedData(5))
fprintf(t, 'DataQuery');

fclose(t);
delete(t);
clear t
    