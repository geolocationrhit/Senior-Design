function [allSensorData] = fetch_all_sensor_data()
    persistent sensors
    
    disp(exist('sensors','var'))
    
    if isempty(sensors)
        disp('Intializing sensors variable')
        sensors = 1;
    else
        data = fetch_sensor_data(sensors)
    end

end