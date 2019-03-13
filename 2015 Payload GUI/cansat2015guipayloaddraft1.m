% Demetrios Doumas Head Programmer   1/24/15
% http://www.eltima.com/products/vspdxp/ for debuging without electronics
function varargout = cansat2015guidraft1(varargin)
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @cansat2015guidraft1_OpeningFcn, ...
                   'gui_OutputFcn',  @cansat2015guidraft1_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end

function cansat2015guidraft1_OpeningFcn(hObject, eventdata, handles, varargin)
handles.output = hObject;
guidata(hObject, handles);



function varargout = cansat2015guidraft1_OutputFcn(hObject, ~, handles) 
varargout{1} = handles.output;

function pushb_Callback(hObject, eventdata, handles)
global s;
startplot=0;
string=transmit;

if (get(hObject,'Value'))
    
    set(handles.pushb, 'String', 'Stop Plot');
    startplot = 1;
   
    
else
    try
    set(handles.pushb, 'String', 'Start Plot');
    delete(instrfindall) % delete all the hidden serial ports
    catch jk
        fclose('all');
        fclose(s);
        delete(s)
        clear s
    end

%cla(handles.Altitude_sensor)
    
end

    

    
if (startplot==1)   

time_arrayalt=[];
time_arrayvolt=[];
time_array_inTemp=[];
time_array_outTemp=[];
time_Xarray=[];
time_Yarray=[];
time_Zarray=[];

inTemp_array=[];
altitude_array=[];
outTemp_array=[];
volt_array=[];
X_array=[];
Y_array=[];
Z_array=[];

% first index
i=1;
g=1;
h=1;
L=1;
K=1;
G=1;
P=1;


indexnum=20; % this gives the length for the array


dlim=',';
TEAM_ID='4996';

fopen(s);% Open serial port
fwrite(s,string);

set(s,'ReadAsyncMode','continuous');% read from the port continuous



        try

            while(1)

            pause(.25);

             dstring=fscanf(s);

                    [token, dstring] = strtok(dstring, dlim);
                    TEAM=token; % stores the team id  string
                    if (strcmp(token ,TEAM_ID)==1)
                     
                        [token, dstring] = strtok(dstring, dlim);
                        mission_time=str2num(token); % stores Packet-count
                        set(handles.time,'string',token); 
                        [token, dstring] = strtok(dstring, dlim);
                        Alt_sensor=str2num(token); % stores mission_time
                        set(handles.text11,'string',Alt_sensor);
            

                        % converts the arrays into a cell
                          time_arrayalt={time_arrayalt}; 
                          altitude_array={altitude_array};
                        if i<=indexnum
                          time_arrayalt{i}=mission_time;
                          altitude_array{i}=Alt_sensor;
                          i=i+1;   
                        else
                            i=1;
                        end

                        % converts to the cell array into math
                        time_arrayalt=cell2mat(time_arrayalt);
                        altitude_array=cell2mat(altitude_array);

                        plot(handles.Altitude_sensor,time_arrayalt,altitude_array,'-or','MarkerSize',5);

                        set(get(handles.Altitude_sensor,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.Altitude_sensor,'Ylabel'),'String','Altitude(m)','color','w')
                        set(get(handles.Altitude_sensor,'Title'),'String','Altitude V.S. Time','color','w')
                        set(handles.Altitude_sensor,'Xcolor','w')
                        set(handles.Altitude_sensor,'Ycolor','w')
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%% OutsideTemperature
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        
                        [token, dstring] = strtok(dstring, dlim);
                        outTemp=str2num(token);
                        set(handles.text12,'string',outTemp);
                        
                           
                        % converts the arrays into a cell
                          time_array_outTemp={time_array_outTemp}; 
                          outTemp_array={outTemp_array};

                          if g<=indexnum
                          time_array_outTemp{g}=mission_time;
                          outTemp_array{g}=outTemp;
                          g=g+1;   
                          else
                            g=1;
                          end

                         time_array_outTemp=cell2mat(time_array_outTemp);
                         outTemp_array=cell2mat(outTemp_array);

                        plot(handles.OutsideTemperature,time_array_outTemp,outTemp_array,'-og','MarkerSize',5);

                        set(get(handles.OutsideTemperature,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.OutsideTemperature,'Ylabel'),'String','Temperature(C)','color','w')
                        set(get(handles.OutsideTemperature,'Title'),'String','TempOutside V.S. Time','color','w')        
                        set(handles.OutsideTemperature,'Xcolor','w')
                        set(handles.OutsideTemperature,'Ycolor','w')
                        
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%% Inside temperature %%%%%%%%%%%%%%%%%%%%
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        [token, dstring] = strtok(dstring, dlim);
                        InTemp=str2num(token);
                        set(handles.text19,'string',InTemp);
                        
                         time_array_inTemp={time_array_inTemp}; 
                         inTemp_array={inTemp_array};

                        if L<=indexnum
                          time_array_inTemp{L}=mission_time;
                          inTemp_array{L}=InTemp;
                          L=L+1;   
                          else
                            L=1;
                         end
                         time_array_inTemp=cell2mat(time_array_inTemp);
                         inTemp_array=cell2mat(inTemp_array);

                        plot(handles.InsideTemperature,time_array_inTemp,inTemp_array,'-om','MarkerSize',5);

                        set(get(handles.InsideTemperature,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.InsideTemperature,'Ylabel'),'String','Temperature(C)','color','w')
                        set(get(handles.InsideTemperature,'Title'),'String','TempInside V.S. Time','color','w')
                        set(handles.InsideTemperature,'Xcolor','w')
                        set(handles.InsideTemperature,'Ycolor','w')

                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        
                        [token, dstring] = strtok(dstring, dlim);
                        voltage=str2num(token);                
                         set(handles.text13,'string',voltage);
                         
                          % converts the arrays into a cell
                          time_arrayvolt={time_arrayvolt}; 
                          volt_array={volt_array};


                         if h<=indexnum
                          time_arrayvolt{h}=mission_time;
                          volt_array{h}=voltage;
                          h=h+1;   
                          else
                          h=1;
                         end
                         time_arrayvolt=cell2mat(time_arrayvolt);
                         volt_array=cell2mat(volt_array);

                        plot(handles.Voltage,time_arrayvolt,volt_array,'-oc','MarkerSize',5);

                        set(get(handles.Voltage,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.Voltage,'Ylabel'),'String','Voltage(v)','color','w')
                        set(get(handles.Voltage,'Title'),'String','Voltage V.S. Time','color','w')
                        set(handles.Voltage,'Xcolor','w')
                        set(handles.Voltage,'Ycolor','w')

                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%%%%%%%%%%%%%%% Flight Software State
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        [token, dstring] = strtok(dstring, dlim);
                         flight_State=str2num(token);
                         set(handles.flightstate,'string',flight_State);
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %%%%%%%% Accelerometer data        
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        [token, dstring] = strtok(dstring, dlim);
                         x=str2num(token);
                         set(handles.text25,'string',x);
                         
                          time_Xarray={time_Xarray}; 
                          X_array={X_array};

                        if K<=indexnum
                          time_Xarray{K}=mission_time;
                          X_array{K}=x;
                          K=K+1;   
                          else
                            K=1;
                         end
                         time_Xarray=cell2mat(time_Xarray);
                         X_array=cell2mat(X_array);

                        plot(handles.X,time_Xarray, X_array,'-om','MarkerSize',5);

                        set(get(handles.X,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.X,'Ylabel'),'String','X-Acceleration(Gs)','color','w')
                        set(get(handles.X,'Title'),'String','X-Acceleration V.S. Time','color','w')
                        set(handles.X,'Xcolor','w')
                        set(handles.X,'Ycolor','w')
                        
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        [token, dstring] = strtok(dstring, dlim);
                         y=str2num(token);
                         set(handles.text26,'string',y);
                         
                         
                          time_Yarray={time_Yarray}; 
                          Y_array={Y_array};

                        if G<=indexnum
                          time_Yarray{G}=mission_time;
                          Y_array{G}=y;
                          G=G+1;   
                          else
                            G=1;
                         end
                         time_Yarray=cell2mat(time_Yarray);
                         Y_array=cell2mat(Y_array);

                        plot(handles.Y,time_Yarray, Y_array,'-om','MarkerSize',5);

                        set(get(handles.Y,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.Y,'Ylabel'),'String','Y-Acceleration(Gs)','color','w')
                        set(get(handles.Y,'Title'),'String','Y-Acceleration V.S. Time','color','w')
                        set(handles.Y,'Xcolor','w')
                        set(handles.Y,'Ycolor','w')
                         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        [token, dstring] = strtok(dstring, dlim);
                         z=str2num(token);
                         set(handles.text27,'string',z);
                        
                         
                          time_Zarray={time_Zarray}; 
                          Z_array={Z_array};

                        if P<=indexnum
                          time_Zarray{P}=mission_time;
                          Z_array{P}=z;
                          P=P+1;   
                          else
                            P=1;
                         end
                         time_Zarray=cell2mat(time_Zarray);
                         Z_array=cell2mat(Z_array);

                        plot(handles.Z,time_Zarray, Z_array,'-om','MarkerSize',5);
                        set(get(handles.Z,'Xlabel'),'String','Time(seconds)','color','w')
                        set(get(handles.Z,'Ylabel'),'String','Z-Acceleration(Gs)','color','w')
                        set(get(handles.Z,'Title'),'String','Z-Acceleration V.S. Time','color','w')
                        set(handles.Z,'Xcolor','w')
                        set(handles.Z,'Ycolor','w')
                        
                        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        %Saving Data to excel
                        % format spec %s string %g remove trialing zeros
                        % fopen 'a' create a new file
                        fileID=fopen('CANSAT2015_TLM_4996.csv','a');
                        fprintf(fileID,'%s, ',TEAM);
                        fprintf(fileID,'%f, ', mission_time, Alt_sensor,outTemp,InTemp, voltage ,flight_State, x, y, z);
                        fprintf(fileID, '\n');
                        fclose(fileID);             


                    else
                        continue;
                    end

            end 




        catch skiperrors
        return;
        end

       
 
    
    
    
end


function popupmenu2_Callback(hObject, eventdata, handles)
global s;
val = get(hObject,'Value');
string_list = get(hObject,'String');
selected_string = string_list{val}; 
s=serial(selected_string, 'BaudRate', 9600,'ReadAsyncMode','continuous');
set(handles.text7, 'String', selected_string);


function popupmenu2_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end





% --- Executes during object creation, after setting all properties.
function axes21_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes22 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes22
rgb = imread('backgroundimage.jpg');
image(rgb, 'XData', [0 .1], 'YData', [0 .1])
set(gca,'xcolor',get(gcf,'color'));
set(gca,'ycolor',get(gcf,'color'));
set(gca,'ytick',[]);
set(gca,'xtick',[]);


% --- Executes during object creation, after setting all properties.
function axes22_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes22 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes22
rgb = imread('Clublogo.jpg');
image(rgb, 'XData', [0 .1], 'YData', [0 .1])
set(gca,'xcolor',get(gcf,'color'));
set(gca,'ycolor',get(gcf,'color'));
set(gca,'ytick',[]);
set(gca,'xtick',[]);
