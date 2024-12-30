assignin("base","target_cells",target_cells);
            writematrix(target_cells,'stimulationTargets.csv');

            if app.SLM_ON && app.SLM == "BNS" % Boulder Nonlinear Systems
                if stimParams.seqMode
%                     phaseMasks = cell(1,size(target_cells,1));
%                     for i = 1:size(target_cells,1)
%                         [phaseMasks{i},~,~] = stimulateBNS_SLM(target_cells(i,:), stimParams); % send commands to BNS SLM
%                     end
                    if size(target_cells_group2,1) > 0 % if there are two groups for stimulation

                        nTargets = 2; % hard coded for now, assuming there are only two groups

                        [phaseMask1,msk1,I1] = stimulateBNS_SLM(target_cells, stimParams); % send commands to BNS SLM
                        pause(1)
                        [phaseMask2,~,~] = stimulateBNS_SLM(target_cells_group2, stimParams); % send commands to BNS SLM
                        pause(1)

                        image_size = size(phaseMask1);
    
                        images_flat(:,1) = reshape(phaseMask1,image_size(1)*image_size(2),1);
                        images_flat(:,2) = reshape(phaseMask2,image_size(1)*image_size(2),1);
                        
                        image_sequence = reshape(images_flat,image_size(1)*image_size(2)*2,1); % stimulate 2 groups of cells in sequence
    
                        disp('Downloading images... please wait.');
                        max_OD_frames = 20;
                        download_error = calllib(app.dll_name, 'Download_image_sequence', app.board_number, nTargets, image_sequence, max_OD_frames);
                        if download_error
                            disp('Failed to download images.');
                        else
                            disp('Done downloading images.');
                            app.StimulateButton.Enable = 1;
                            imshow(msk1,'Parent', app.UIAxes8); % for now only display the phase mask corresponding to the first vortex
                            imshow(I1,'Parent', app.UIAxes8_2);
                        end

                    else
                    
                        [phaseMask1,msk1,I1] = stimulateBNS_SLM(target_cells(1,:), stimParams); % send commands to BNS SLM
                        pause(1)
                        [phaseMask2,~,~] = stimulateBNS_SLM(target_cells(2,:), stimParams); % send commands to BNS SLM
                        pause(1)
                        if nTargets > 2
                            [phaseMask3,~,~] = stimulateBNS_SLM(target_cells(3,:), stimParams); % send commands to BNS SLM
                            if nTargets == 4
                                [phaseMask4,~,~] = stimulateBNS_SLM(target_cells(4,:), stimParams); % send commands to BNS SLM
                            end
                        end
    
                        image_size = size(phaseMask1);
    
                        images_flat(:,1) = reshape(phaseMask1,image_size(1)*image_size(2),1);
                        images_flat(:,2) = reshape(phaseMask2,image_size(1)*image_size(2),1);
    
                        if nTargets == 4
                            images_flat(:,3) = reshape(phaseMask3,image_size(1)*image_size(2),1);
                            images_flat(:,4) = reshape(phaseMask4,image_size(1)*image_size(2),1);
                            image_sequence = reshape(images_flat,image_size(1)*image_size(2)*4,1); % stimulate 4 cells in sequence
                        elseif nTargets == 3
                            images_flat(:,3) = reshape(phaseMask3,image_size(1)*image_size(2),1);
                            image_sequence = reshape(images_flat,image_size(1)*image_size(2)*3,1); % stimulate 3 cells in sequence
                        else
                            image_sequence = reshape(images_flat,image_size(1)*image_size(2)*2,1); % stimulate 2 cells in sequence
                        end
    
                        disp('Downloading images... please wait.');
                        max_OD_frames = 20;
                        download_error = calllib(app.dll_name, 'Download_image_sequence', app.board_number, nTargets, image_sequence, max_OD_frames);
                        if download_error
                            disp('Failed to download images.');
                        else
                            disp('Done downloading images.');
                            app.StimulateButton.Enable = 1;
                            imshow(msk1,'Parent', app.UIAxes8); % for now only display the phase mask corresponding to the first vortex
                            imshow(I1,'Parent', app.UIAxes8_2);
                        end
                    end
                end
            end