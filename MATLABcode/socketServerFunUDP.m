% [lat, long, time, heading, RSS] = fetch_sensor_data(sensor_id)

u = udp('127.0.0.1', 5006, 'LocalPort', 5006)
set(u,'Terminator','LF')
fopen(u);

%fprintf(t, 'DataQuery');

%pause(1);

%dataReceived = fscanf(t)
%set(t,'Terminator','LF')

%{
while(t.bytesAvailable == 0 )
    
end
%}

%dataReceived = fread(t,t.bytesAvailable,'char')
while(1)
    while(u.bytesAvailable == 0)

    end
    
    dataReceived = fscanf(u)
end


while (get(u,'BytesAvailable') > 0)
        u.BytesAvailable
        dataReceived = fscanf(u)
end

%parsedData = sscanf(dataReceived,'LAT:%f LONG:%f TIME:%d HEADING:%f RSS:%f')
%dataStruct = struct('lat', parsedData(1),'long',parsedData(2),'time',parsedData(3),'heading',parsedData(4),'RSS',parsedData(5))
disp('Sending DataQuery')
fprintf(u, 'DataQuery');

fclose(u);
delete(u);
clear u
    