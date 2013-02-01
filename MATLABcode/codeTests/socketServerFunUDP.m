% [lat, long, time, heading, RSS] = fetch_sensor_data(sensor_id)

u = udp('127.0.0.1', 5007, 'LocalPort', 5007)
u.Timeout = 3.0;
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
startTime = clock;
newTime = clock;
%dataReceived = fread(t,t.bytesAvailable,'char')
sensors = {};
i = 1;
while(etime(newTime,startTime) < 10)
    %{
    while(u.bytesAvailable == 0 && etime(newTime,startTime) < 30)
        newTime = clock;
    end
    %}
    
    dataReceived = fscanf(u);
    if(~isempty(dataReceived))
       arrayOfIPs = [sensors{:}];
       if(isempty(strfind(arrayOfIPs,dataReceived)));
           sensors{i} = dataReceived; 
           i = i + 1;
       end
    end
    newTime = clock;
end


while (get(u,'BytesAvailable') > 0)
        u.BytesAvailable
        dataReceived = fscanf(u)
end

%parsedData = sscanf(dataReceived,'LAT:%f LONG:%f TIME:%d HEADING:%f RSS:%f')
%dataStruct = struct('lat', parsedData(1),'long',parsedData(2),'time',parsedData(3),'heading',parsedData(4),'RSS',parsedData(5))
disp('Sending DataQuery')
fprintf(u, 'DataQuery\n');

fclose(u);
delete(u);
clear u
    