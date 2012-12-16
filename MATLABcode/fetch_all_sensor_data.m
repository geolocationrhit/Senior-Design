function [allSensorData] = fetch_all_sensor_data(sensor_addresses)
    %persistent sensors
    
    %disp(exist('sensors','var'))
    
    if isempty(sensor_addresses)
        disp('No sensors to fetch data from')
        %disp('Intializing sensors variable')
        %sensors = 1;
    else
        for sensor=sensor_addresses;
            disp(['Fetching data from: ',sensor{1}])
            data = fetch_sensor_data(sensor{1})
        end
        
    end

end