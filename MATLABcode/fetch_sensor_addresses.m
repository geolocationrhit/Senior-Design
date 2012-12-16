function [sensor_addresses] = fetch_sensor_addresses()
    u = udp('127.0.0.1', 5007, 'LocalPort', 5007);
    u.Timeout = 3.0;
    set(u,'Terminator','LF')
    fopen(u);

    startTime = clock;
    newTime = clock;

    sensor_addresses = {};
    i = 1;
    while(etime(newTime,startTime) < 10)
        dataReceived = fscanf(u);
        if(~isempty(dataReceived))
           arrayOfIPs = [sensor_addresses{:}];
           if(isempty(strfind(arrayOfIPs,dataReceived)));
               sensor_addresses{i} = dataReceived; 
               i = i + 1;
           end
        end
        newTime = clock;
    end


    while (get(u,'BytesAvailable') > 0)
            u.BytesAvailable;
            dataReceived = fscanf(u);
    end

    %parsedData = sscanf(dataReceived,'LAT:%f LONG:%f TIME:%d HEADING:%f RSS:%f')
    %dataStruct = struct('lat', parsedData(1),'long',parsedData(2),'time',parsedData(3),'heading',parsedData(4),'RSS',parsedData(5))
    %disp('Sending DataQuery')
    fprintf(u, 'DataQuery\n');

    fclose(u);
    delete(u);
    clear u
end