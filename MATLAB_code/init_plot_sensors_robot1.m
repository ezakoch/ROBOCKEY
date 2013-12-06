function init_plot_sensors_robot1( src,evnt )

global userdata
    
        % Initialiaze figures
        fig4 = figure(4);
        set(fig4,'Position',[0 0 1200 800])

        subplot(2,1,1)
        hold all
        
        userdata.ADC.robot1.plot_PT_LO = plot(0,0);
        userdata.ADC.robot1.plot_PT_LI = plot(0,0);
        userdata.ADC.robot1.plot_PT_RI = plot(0,0);
        userdata.ADC.robot1.plot_PT_RO = plot(0,0);
        userdata.ADC.robot1.plot_PT_BL = plot(0,0);
        userdata.ADC.robot1.plot_PT_BR = plot(0,0);
        userdata.ADC.robot1.plot_PT_PUCK = plot(0,0);
        
        
        title('Sensors')
        xlabel('Time (sec)')
        ylabel('ADC (0-1023)')
        legend([userdata.ADC.robot1.plot_PT_LO...
                userdata.ADC.robot1.plot_PT_LI...
                userdata.ADC.robot1.plot_PT_RI...
                userdata.ADC.robot1.plot_PT_RO...
                userdata.ADC.robot1.plot_PT_BL...
                userdata.ADC.robot1.plot_PT_BR...
                userdata.ADC.robot1.plot_PT_PUCK],...
            'LO','LI','RI','RO','BL','BR','PUCK','Location','NorthEastOutside')
        ylim([-100 1124])
        grid on
        hold off
        
        subplot(2,2,4)
        hold on
        plot_arc(60*pi/180,-240*pi/180,0,0,1)
        plot_arc(-25*pi/180,-155*pi/180,0,1.1,0.55)
                
        LO = plot(-sqrt(2)/2,sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(-sqrt(2)/2,sqrt(2)/2,'   LO','FontSize',12)
        LI = plot(-0.3,0.55,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(-0.3,0.55,'   LI','FontSize',12)
        
        RI = plot(0.3,0.55,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(0.3,0.55,'   RI','FontSize',12)
        RO = plot(sqrt(2)/2,sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(sqrt(2)/2,sqrt(2)/2,'   RO','FontSize',12)
        
        BL = plot(-sqrt(2)/2,-sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(-sqrt(2)/2,-sqrt(2)/2,'   BL','FontSize',12)
        BR = plot(sqrt(2)/2,-sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(sqrt(2)/2,-sqrt(2)/2,'   BR','FontSize',12)
        
        PUCK = plot(0,0.3,'bs','MarkerSize',10,'MarkerFaceColor','b');
        text(0,0.3,'   P','FontSize',12)
        
        userdata.ADC.robot1.sensors = [LO LI RI RO BL BR PUCK];
        userdata.ADC.robot1.ids = 1:6;
        
        hold off
        ylim([-1.5 1.5])
        ylim([-1.5 1.5])
        axis equal
        grid off
        
        
        text_x = 100;
        text_y = 40;
        
        h.robot_num = uicontrol('Style', 'text', 'String', 'Robot 1','Fontsize',18,...
            'Position', [300 8*text_y 150 20],'BackgroundColor',userdata.defaultBackground);
        
        h.PT_LO = uicontrol('Style', 'text', 'String', 'LO','Fontsize',18,...
            'Position', [text_x 8*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.PT_LI = uicontrol('Style', 'text', 'String', 'LI','Fontsize',18,...
            'Position', [text_x 7*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.PT_RI = uicontrol('Style', 'text', 'String', 'RI','Fontsize',18,...
            'Position', [text_x 6*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.PT_RO = uicontrol('Style', 'text', 'String', 'RO','Fontsize',18,...
            'Position', [text_x 5*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.PT_BL = uicontrol('Style', 'text', 'String', 'BL','Fontsize',18,...
            'Position', [text_x 4*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.PT_BR = uicontrol('Style', 'text', 'String', 'BR','Fontsize',18,...
            'Position', [text_x 3*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.PT_PUCK = uicontrol('Style', 'text', 'String', 'PUCK','Fontsize',18,...
            'Position', [text_x 2*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        
        text_x_value = 60;
        userdata.ADC.robot1.PT_LO_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 8*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.ADC.robot1.PT_LI_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 7*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.ADC.robot1.PT_RI_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 6*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.ADC.robot1.PT_RO_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 5*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.ADC.robot1.PT_BL_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 4*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.ADC.robot1.PT_BR_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 3*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.ADC.robot1.PT_PUCK_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 2*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
       

end

