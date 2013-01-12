format longg
%fetch_all_sensor_data()
sensor_addresses = fetch_sensor_addresses()
for i=1:length(sensor_addresses); % move this for loop into fetch_all_sensor data
    fetch_sensor_data(sensor_addresses{i})
    %fetch_all_sensor_data(sensor_addresses)
end
%fetch_all_sensor_data(sensor_addresses)