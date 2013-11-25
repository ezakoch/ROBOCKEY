function debugging()

global userdata

% Send M2 the instruction to send enemies data
fwrite(userdata.handle, 7);

% Check if we have available bytes from M2
nBytes = userdata.handle.BytesAvailable;
while(nBytes == 0)
    nBytes = userdata.handle.BytesAvailable;
end


% Grab data from M2 until line terminator
data = fgetl(userdata.handle);
userdata.debugging.status_gtg = str2double(data);
set(userdata.h.var1, 'String',userdata.debugging.status_gtg)

data = fgetl(userdata.handle);
userdata.debugging.dirx = str2double(data);
set(userdata.h.var2, 'String',userdata.debugging.dirx)

data = fgetl(userdata.handle);
userdata.debugging.diry = str2double(data);
set(userdata.h.var3, 'String',userdata.debugging.diry)

data = fgetl(userdata.handle);
userdata.debugging.angle_to_goal = str2double(data);
set(userdata.h.var4, 'String',userdata.debugging.angle_to_goal)

data = fgetl(userdata.handle);
userdata.debugging.dist_to_goal = str2double(data);
set(userdata.h.var5, 'String',userdata.debugging.dist_to_goal)

drawnow;

end




