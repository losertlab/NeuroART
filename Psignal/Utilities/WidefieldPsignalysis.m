function varargout = WidefieldPsignalysis(varargin)
%%%%%%%% Setup environment %%%%%%%%
username=getenv('USERNAME');
addpath(genpath(['C:\Users\'  username '\Dropbox\Psignal\Psignal']))
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @WidefieldPsignalysis_OpeningFcn, ...
    'gui_OutputFcn',  @WidefieldPsignalysis_OutputFcn, ...
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
function WidefieldPsignalysis_OpeningFcn(hObject, eventdata, handles, varargin)
handles.output = hObject;
guidata(hObject, handles);
set(handles.figure1,'toolbar','figure');
set(handles.figure1,'menubar','figure');
function varargout = WidefieldPsignalysis_OutputFcn(hObject, eventdata, handles)
%%%%%%%% Select files for analysis %%%%%%%%
function loadsurface_Callback(hObject, eventdata, handles)
[surfacefileName surfaceFilePath] = uigetfile('*.*','Load Surface Images');
handles.surfacefile = [surfaceFilePath surfacefileName];
handles.surfaceFilePath = surfaceFilePath;
guidata(hObject, handles);
function loaddeltaf_Callback(hObject, eventdata, handles)
if isfield(handles,'surfaceFilePath') && isstr(handles.surfaceFilePath)
    [deltaFfileName deltaFFilePath] = uigetfile('*.*','Load DeltaF Images',handles.surfaceFilePath,'MultiSelect','on');
else
    [deltaFfileName deltaFFilePath] = uigetfile('*.*','Load DeltaF Images','MultiSelect','on');
end
handles.deltaFfile = [deltaFFilePath deltaFfileName];
guidata(hObject, handles);
function loadpsignal_Callback(hObject, eventdata, handles)
if isfield(handles,'surfaceFilePath') && isstr(handles.surfaceFilePath)
    [PsignalfileName PsignalFilePath] = uigetfile('.mat','Load Psignal Data',handles.surfaceFilePath);
else
    [PsignalfileName PsignalFilePath] = uigetfile('.mat','Load Psignal Data');
end
handles.Psignalfile = [PsignalFilePath PsignalfileName];
guidata(hObject, handles);
function go_Callback(hObject, eventdata, handles)
%Free parameters
hmcut_Callback(hObject, eventdata, handles)
hmcut = get(handles.hmcut,'Value');
SpatFilt_Callback(hObject, eventdata, handles)
SpatFilt = get(handles.SpatFilt,'Value');
DSFact_Callback(hObject, eventdata, handles)
DSFact = get(handles.DSFact,'Value');
hwrect_Callback(hObject, eventdata, handles)
hwrect = get(handles.hwrect,'Value');
%%%%%%%% Load Psignal data and extract parameters %%%%%%%%
if ~isfield(handles,'surfacefile') || ~isfield(handles,'deltaFfile') || ~isfield(handles,'Psignalfile')
    warndlg('Must load (1) Surface file, (2) DeltaF file and (3) Psignal file')
    return
end
clc
load(handles.Psignalfile);
%Keyword stimulus selection
keyword = get(handles.keyword,'String');
%Frequency and Level order
OveralldB =get(exptparams.TrialObject,'OveralldB');
FreqLevelOrder=[];
stimypes=[];
t=1;
keystim=[];
AttenRange = get(get(exptparams.TrialObject,'PrimaryHandle'),'AttenRange');
for i = 1:length(exptevents)
    strparts = strsep(exptevents(i).Note,',',1);
    if strcmpi(deblank(strparts{1}),'Stim')
        if sum(AttenRange) > 0
            FreqLevelOrder = [FreqLevelOrder; str2num(strparts{2}) OveralldB-str2num(strparts{4})];
        else
            FreqLevelOrder = [FreqLevelOrder; str2num(strparts{2}) OveralldB];
        end
        if strcmpi(exptparams.runclass,'AHL')
            stimypes = [stimypes; strparts(3)];
        elseif strcmpi(exptparams.runclass,'ART')
            stimypes = [stimypes; strparts(3)];
        elseif strcmpi(exptparams.runclass,'RND')
            stimypes = [stimypes; strparts(3)];
        else 
            stimypes = [stimypes; strparts(3)];
        end 
        if ~isempty(find(~cellfun(@isempty,strfind(strparts,keyword))))
            keystim = [keystim; t];
        end
        t=t+1;
    end
end
%Noise or tones
NoiseOrTones =get(get(exptparams.TrialObject,'PrimaryHandle'),'Type');
handles.noise = 0;
if strcmpi(NoiseOrTones, 'WhiteNoise')
    handles.noise = 1;
end
handles.keystim = keystim;
handles.Freqs=unique(FreqLevelOrder(:,1));
handles.Levels=unique(FreqLevelOrder(:,2));
pfs=str2num(globalparams.PhysHz);
handles.pfs=pfs;
handles.PrimaryDuration = get(get(exptparams.TrialObject,'PrimaryHandle'),'Duration');
handles.PreStimSilence = get(get(exptparams.TrialObject,'PrimaryHandle'),'PreStimSilence');
handles.PostStimSilence = get(get(exptparams.TrialObject,'PrimaryHandle'),'PostStimSilence');
framespertrial = pfs*(handles.PreStimSilence+handles.PrimaryDuration+handles.PostStimSilence);
%%%%%%%% Load surface data %%%%%%%%
disp(' ')
disp('Loading Surface')
disp(' ')
surfaces=[];
if isempty(strfind(handles.surfacefile,'tif'))
    framepix = 20*DSFact;
    %     shift=64;
    shift=0;
    [headerInfo, surfaces1] = strmpix2matlab(handles.surfacefile,0);
    disp(' ')
    for i = 1:size(surfaces1,3)
        surfaces(:,:,i) = imresize(circshift(squeeze(surfaces1(:,:,i))',shift)',DSFact);
    end
    surfaces = surfaces(framepix:end-framepix-1,framepix:end-framepix-1,:);
else
    framepix = 64*DSFact;
    shift=0;
    warning('OFF', 'MATLAB:imagesci:tiffmexutils:libtiffWarning')
    warning('OFF', 'MATLAB:imagesci:tifftagsread:nextIfdPointerOutOfRange')
    InfoImage=imfinfo(handles.surfacefile);
    mImage=InfoImage(1).Width;
    nImage=InfoImage(1).Height;
    NumberImages=length(InfoImage);
    surfaces=zeros(mImage*DSFact,nImage*DSFact,NumberImages,'uint16');
    TifLink = Tiff(handles.surfacefile, 'r');
    for i=1:NumberImages
        TifLink.setDirectory(i);
        surfaces(:,:,i)=imresize(rot90(TifLink.read()),DSFact);
    end
    TifLink.close();
    surfaces = surfaces(framepix:end-framepix-1,:,:);
end
homof=1;
fc=1;
surface = DeltaF(surfaces, 1, homof, fc, size(surfaces,3));
disp(' ')
handles.surface=imsharpen(adapthisteq(surface./max(max(abs(surface)))));
%%%%%%%% Load flourescence data %%%%%%%%
disp(' ')
disp('Loading Flourescence')
I=[];
if isempty(strfind(handles.surfacefile,'tif'))
    %Data came from .seq file, usually from Streampix in the Kanold Lab
    fileName = [mfilename '.seq'];
    [headerInfo, I1] = strmpix2matlab(handles.deltaFfile,0,DSFact);
    disp(' ')
    for i = 1:size(I1,3)
        I(:,:,i) = circshift(squeeze(I1(:,:,i))',shift)';
    end
    %Take off edges of images, and only keep expected number of images.
    TrialsPerFreq = get(exptparams.TrialObject,'TrialsPerFreq');
    I = I(framepix:end-framepix-1,framepix:end-framepix-1,1:framespertrial*TrialsPerFreq*length(handles.Freqs)*length(handles.Levels));
else
    %Data came from .tiff stack, usutall from ThorCam in the Kanold Lab
    I=[];
    framecount=0;
    if iscell(handles.deltaFfile)
        for ii = 2:length(handles.deltaFfile)
            InfoImage=imfinfo([handles.deltaFfile{1} handles.deltaFfile{ii}]);
            NumberImages=length(InfoImage);
            TifLink = Tiff([handles.deltaFfile{1} handles.deltaFfile{ii}], 'r');
            for i=1:NumberImages
                TifLink.setDirectory(i);
                framecount=framecount+1;
                I(:,:,framecount)=imresize(rot90(TifLink.read()),DSFact);
                disp(['Loaded frame ' num2str(framecount)])
            end
            TifLink.close();
        end
    else
        InfoImage=imfinfo([handles.deltaFfile]);
        NumberImages=length(InfoImage);
        TifLink = Tiff([handles.deltaFfile], 'r');
        for i=1:NumberImages
            TifLink.setDirectory(i);
            framecount=framecount+1;
            I(:,:,framecount)=imresize(rot90(TifLink.read()),DSFact);
            disp(['Loaded frame ' num2str(framecount)])
        end
        TifLink.close();
    end
    %Take off edges of images, and only keep expected number of images.
    if isfield(get(exptparams.TrialObject),'TrialsPerFreq')
        TrialsPerFreq = get(exptparams.TrialObject,'TrialsPerFreq');
    else
        TrialsPerFreq = inputdlg('Enter Trials per Frequency');
        if isempty(TrialsPerFreq)
            return
        else
            TrialsPerFreq = str2num(TrialsPerFreq{1});
        end
    end
    try
        I = I(framepix:end-framepix-1,:,1:length(unique(stimypes))*framespertrial*TrialsPerFreq*length(handles.Freqs)*length(handles.Levels));
    catch
        warndlg('Trials per Freq incorrect!')
        return
    end
end
if isempty(TrialsPerFreq)
    return
end
%Rearrange by trial
I = reshape(I,[size(I,1) size(I,2) framespertrial length(unique(stimypes))*TrialsPerFreq*length(handles.Freqs)*length(handles.Levels)]);
keystim = handles.keystim(find(handles.keystim<size(I,4)));
FreqLevelOrder = FreqLevelOrder(1:length(unique(stimypes))*TrialsPerFreq*length(handles.Freqs)*length(handles.Levels),:);
FreqLevelOrder = FreqLevelOrder(keystim,:);
if isempty(FreqLevelOrder)
    return
end
%Rearrange by level x frequency
ILF=[]; %5d matrix: pixel x pixel x frame x level x frequency
for i = 1:length(handles.Levels)
    Lidx = find(FreqLevelOrder(:,2)==handles.Levels(i));
    for ii = 1:length(handles.Freqs)
        FLidx = find(FreqLevelOrder(Lidx,1)==handles.Freqs(ii));
        ILF(:,:,:,i,ii) = squeeze(nanmean(I(:,:,:,Lidx(FLidx)),4));
    end
end
%Find DeltaF
if hmcut > 0
    homof=1;
    fc=hmcut;
else
    homof=0;
    fc=hmcut;
end
spatialfilter = fspecial('average',SpatFilt);
baseline=[];
DeltaFF=[];
for i = 1:length(handles.Levels)
    for ii = 1:length(handles.Freqs)
        disp(' ')
        disp(['Filtering: ' num2str(handles.Levels(i)) 'dB; ' num2str(handles.Freqs(ii)) 'Hz'])
        [baseline(:,:,i,ii) DeltaFF(:,:,:,i,ii)] = DeltaF(squeeze(ILF(:,:,:,i,ii)), pfs, homof, fc, handles.PreStimSilence);
        baseline(:,:,i,ii) = filter2(spatialfilter,baseline(:,:,i,ii));
        for iii = 1:size(DeltaFF,3)
            DeltaFF(:,:,iii,i,ii) = filter2(spatialfilter, DeltaFF(:,:,iii,i,ii));
            if hwrect
                DeltaFF(:,:,iii,i,ii) = DeltaFF(:,:,iii,i,ii).*(DeltaFF(:,:,iii,i,ii) >= 0);
            end
        end
    end
end
handles.DeltaFF = DeltaFF;
%%%%%%%% Plot surface %%%%%%%%
handles.surfaceAP = rot90(handles.surface,-1);
cla(handles.surfacefig,'reset')
imshow(handles.surfaceAP,[],'Parent',handles.surfacefig);
title(['Surface'],'Parent',handles.surfacefig)
hold on
T=text(20,size(handles.surfaceAP,1)/2,'A','fontsize',20,'fontweight','bold','color','w','Parent',handles.surfacefig);
set(T, 'rotation', 90)
text(size(handles.surfaceAP,1)/2,20,'M','fontsize',20,'fontweight','bold','color','w','Parent',handles.surfacefig);
set(gca,'fontsize',8)
%%%%%%%% Plot DeltaF %%%%%%%%
RespWin_Callback(hObject, eventdata, handles)
ResponseWin=get(handles.RespWin,'Value');
idx=ceil(handles.PreStimSilence*pfs+1:handles.PreStimSilence*pfs+(ResponseWin*pfs));
handles.muDeltaFFAP=rot90(squeeze(nanmean(nanmean(nanmean(DeltaFF(:,:,idx,:,:),3),4),5)),-1);
cla(handles.deltaFfig,'reset')
imshow((squeeze(nanmean(handles.muDeltaFFAP,3))./max(max(abs(squeeze(nanmean(handles.muDeltaFFAP,3)))))),[],'Parent',handles.deltaFfig);
title(['Mean \DeltaF'],'Parent',handles.deltaFfig)
colormap(handles.deltaFfig,gray)
set(handles.stimuluslevel,'String',handles.Levels)
set(handles.analysis,'String',[{'Time-course'};{'Frequency Response Area'}])
guidata(hObject, handles)
function pixel_Callback(hObject, eventdata, handles)
[y,x] = ginput;
handles.pixely=round(y);
handles.pixelx=round(x);
guidata(hObject, handles)
function stimuluslevel_Callback(hObject, eventdata, handles)
%Select data from a given stimulus level
handles.idxdB = get(handles.stimuluslevel,'Value');
guidata(hObject, handles)
function stimuluslevel_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function analysis_Callback(hObject, eventdata, handles)
guidata(hObject, handles)
function analysis_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function drawmask_Callback(hObject, eventdata, handles)
cla(handles.surfacefig,'reset')
axes(handles.surfacefig)
surfmask=imshow(handles.surfaceAP,[],'Parent',handles.surfacefig);
title(['Surface'],'Parent',handles.surfacefig)
hold on
T=text(20,size(handles.surfaceAP,1)/2,'A','fontsize',20,'fontweight','bold','color','w','Parent',handles.surfacefig);
set(T, 'rotation', 90)
text(size(handles.surfaceAP,1)/2,20,'M','fontsize',20,'fontweight','bold','color','w','Parent',handles.surfacefig);
h = imellipse(handles.surfacefig);
mask = double(createMask(h,surfmask));
delete(h);
mask(find(mask==0))=nan;
if ~isempty(mask)
    handles.mask = mask;
else
    return
end
imshow(handles.surfaceAP.*mask,[],'Parent',handles.surfacefig)
hold on
title(['Surface'],'Parent',handles.surfacefig)
hold on
T=text(20,size(handles.surfaceAP,1)/2,'A','fontsize',20,'fontweight','bold','color','w','Parent',handles.surfacefig);
set(T, 'rotation', 90)
text(size(handles.surfaceAP,1)/2,20,'M','fontsize',20,'fontweight','bold','color','w','Parent',handles.surfacefig);

colormap gray ; freezeColors; 

guidata(hObject, handles)
function drawroi_Callback(hObject, eventdata, handles)
cla(handles.deltaFfig,'reset')
axes(handles.deltaFfig)
if ~isfield(handles,'mask')
    handles.mask = ones(size(handles.muDeltaFFAP));
end
ROI = double(roipoly(double(handles.mask.*(squeeze(nanmean(handles.muDeltaFFAP,3))./max(max(abs(squeeze(nanmean(handles.muDeltaFFAP,3)))))))));
colormap(handles.deltaFfig,gray)
ROI(find(ROI==0))=nan;
handles.ROI = ROI;
imshow((handles.muDeltaFFAP).*handles.mask,[],'Parent',handles.deltaFfig);
hold on
title(['Mean \DeltaF'],'Parent',handles.deltaFfig)
colormap(handles.deltaFfig,gray)
freezeColors(handles.deltaFfig)
guidata(hObject, handles)
function plottonotopy_Callback(hObject, eventdata, handles)
cla(handles.tonotopyfig,'reset')
axes(handles.tonotopyfig)
idx = floor(handles.pfs*(handles.PreStimSilence:1/handles.pfs:handles.PreStimSilence+get(handles.RespWin,'Value')));
if isfield(handles,'idxdB')
    handles.muDeltaFdB = squeeze(nanmean(handles.DeltaFF(:,:,idx,handles.idxdB,:),3));
else
    warndlg('Need to select sound level (dB)')
    return
end
handles.muDeltaFdBAP = zeros(size(handles.muDeltaFdB));
for i = 1:size(handles.muDeltaFdB,3)
    handles.muDeltaFdBAP(:,:,i) = rot90(squeeze(handles.muDeltaFdB(:,:,i)),-1);
end
thresh_Callback(hObject, eventdata, handles)
handles.threshval = get(handles.thresh,'Value');
threshold = max(max(squeeze(max(handles.muDeltaFdBAP.*repmat(handles.ROI,[1 1 size(handles.muDeltaFdBAP,3)])))));
%Find tonotopy
Tonotopy=zeros(size(handles.surfaceAP));
TonotopyVal=zeros(size(handles.surfaceAP));
treshval = handles.threshval;
for i = 1:size(handles.muDeltaFdBAP,1)
    for ii = 1:size(handles.muDeltaFdBAP,2)
        disp(['Processing Pixel (' num2str(i) ',' num2str(ii) ')'])
        f=find(squeeze(handles.muDeltaFdBAP(i,ii,:))>= treshval*threshold);
        val=handles.muDeltaFdBAP(i,ii,f);
        if ~isempty(f)
            if ~handles.noise
                Tonotopy(i,ii) = median(f);
            else
                Tonotopy(i,ii) = 1;
            end
            TonotopyVal(i,ii) = median(val);
        else
            Tonotopy(i,ii) = nan;
            TonotopyVal(i,ii) = 0;
        end
    end
end
Tonotopy = Tonotopy.*handles.mask;
handles.FreqKept = unique(round(Tonotopy(~isnan(Tonotopy))));
[r c] = find(isnan(Tonotopy./max(max(abs(Tonotopy)))));
Tonotopy = Tonotopy./length(handles.Freqs);
TonotopyScaled = uint8(256*Tonotopy);
TonotopyRGB = ind2rgb(TonotopyScaled,jet(256));
SurfaceRGB = cat(3,handles.surfaceAP./max(max(abs(handles.surfaceAP))), handles.surfaceAP./max(max(abs(handles.surfaceAP))),handles.surfaceAP./max(max(abs(handles.surfaceAP))));
TonotopicMap=TonotopyRGB;
for i = 1:length(r)
    TonotopicMap(r(i),c(i),:) = SurfaceRGB(r(i),c(i),:);
end
if ~isfield(handles,'mask')
    handles.mask = ones(size(handles.muDeltaFFAP));
end
TonotopicMap = TonotopicMap.*cat(3,handles.mask,handles.mask,handles.mask);
imshow(handles.surfaceAP.*handles.mask,[],'Parent',handles.tonotopyfig)
hold on
TonotopyVal = TonotopyVal.*handles.mask;
if get(handles.magw,'Value')
    imagesc(TonotopicMap,'Parent',handles.tonotopyfig,'AlphaData',(TonotopyVal-nanmin(nanmin(TonotopyVal)))./max(max(abs(TonotopyVal-nanmin(nanmin(TonotopyVal))))));
    hold on
    imagesc(TonotopicMap,'Parent',handles.tonotopyfig,'AlphaData',(TonotopyVal-nanmin(nanmin(TonotopyVal)))./max(max(abs(TonotopyVal-nanmin(nanmin(TonotopyVal))))));
else
    imagesc(TonotopicMap,'Parent',handles.tonotopyfig);
end
h=colorbar(gca,'location','eastoutside');
colormap(h,jet)
lim=get(h,'Limits');
if ~handles.noise
    set(h,'ytick',linspace(min(lim),max(lim),length(handles.Freqs)),'yticklabel',roundTo(handles.Freqs./1000,2));
    ylabel(h,'Frequency (kHz)')
else
colorbar('off')
end
title(handles.tonotopyfig,['Frequency Map'])
set(gca,'fontsize',8)
freezeColors
hold off
guidata(hObject, handles)
function plotanalysis_Callback(hObject, eventdata, handles)
%Plot analysis for a given pixel
analyses = get(handles.analysis,'String');
analysis = get(handles.analysis,'Value');
analysis = analyses{analysis};
axes(handles.analysisfig)
cla(handles.analysisfig,'reset')
if strcmpi(analysis,'Time-course')
    %Time-course
    DeltaFFAP = zeros(size(handles.DeltaFF));
    for i = 1:size(DeltaFFAP,3)
        for ii = 1:size(DeltaFFAP,4)
            for iii = 1:size(DeltaFFAP,5)
                DeltaFFAP(:,:,i,ii,iii) = rot90(handles.DeltaFF(:,:,i,ii,iii),-1);
            end
        end
    end
    PointDeltaFFdBTime = squeeze(nanmean(nanmean(DeltaFFAP(handles.pixelx',handles.pixely',:,handles.idxdB,:),1),2));
    interpfact=2;
    pfs=handles.pfs;
    t=-handles.PreStimSilence:1/(pfs*interpfact):(size(PointDeltaFFdBTime,1)/pfs)-(1/pfs*interpfact);
    f=linspace(handles.Freqs(1), handles.Freqs(end),interpfact*length(handles.Freqs));
    InterpTC = interp2(PointDeltaFFdBTime, interpfact);
    imagesc(t,f,InterpTC'.*100,'Parent',handles.analysisfig);
    set(gca,'YDir','normal');
    set(handles.analysisfig,'clim',[0 max(max(abs(InterpTC.*100)))]);
    hold on
    aa=axis;
    hold on
    plot([0 0],[aa(3) aa(4)],'w:','linewidth',2)
    colormap(handles.analysisfig,jet)
    h=colorbar('eastoutside');
    ylabel(h,'\DeltaF (%)')
    ylabel('Frequency (kHz)')
    xlabel('Time (s re. tone onset)')
    set(gca,'fontsize',8)
    guidata(hObject, handles)
elseif strcmpi(analysis,'Frequency Response Area')
    %FRA
    idx = floor(handles.pfs*(handles.PreStimSilence:1/handles.pfs:handles.PreStimSilence+get(handles.RespWin,'Value')));
    DeltaFFAP = zeros(size(handles.DeltaFF));
    for i = 1:size(DeltaFFAP,3)
        for ii = 1:size(DeltaFFAP,4)
            for iii = 1:size(DeltaFFAP,5)
                DeltaFFAP(:,:,i,ii,iii) = rot90(handles.DeltaFF(:,:,i,ii,iii),-1);
            end
        end
    end
    fra = squeeze(nanmean(nanmean(nanmean(DeltaFFAP(handles.pixelx',handles.pixely',idx,:,:),1),2),3))*100;
    if length(handles.Levels) > 1
        contourf(roundTo(handles.Freqs(:)./1000,1),handles.Levels,fra,'Parent',handles.analysisfig);
        colormap(jet)
        ylabel('Level (dB SPL)')
    else
        plot(roundTo(handles.Freqs(:)./1000,1),fra,'k','linewidth',2,'Parent',handles.analysisfig);
        ylabel('\DeltaF/F (%)')
        axis tight
    end
    xlabel('Frequency (kHz)')
    set(gca,'fontsize',8)
    guidata(hObject, handles)
end
%Free parameters
function RespWin_Callback(hObject, eventdata, handles)
s=get(handles.RespWin,'String');
if strcmpi(s,'') || isempty(s)
    set(handles.RespWin,'Value',max([handles.PrimaryDuration+handles.PostStimSilence-1 handles.PrimaryDuration]));
    set(handles.RespWin,'String',num2str(max([handles.PrimaryDuration+handles.PostStimSilence-1 handles.PrimaryDuration])));
else
    s = str2num(s);
    if s > handles.PrimaryDuration+handles.PostStimSilence
        warndlg('Invalid Response Window')
        return
        set(handles.RespWin,'String','');
    else
        set(handles.RespWin,'String',num2str(s));
        set(handles.RespWin,'Value',s);
    end
end
guidata(hObject, handles)
function RespWin_CreateFcn(hObject, eventdata, handles)
function thresh_Callback(hObject, eventdata, handles)
s=get(handles.thresh,'String');
if strcmpi(s,'') || isempty(s)
    set(handles.thresh,'Value',0.2);
    set(handles.thresh,'String',num2str(0.2));
else
    s = str2num(s);
    if s < 0 || s > 1
        warndlg('Invalid threshold')
        return
        set(handles.thresh,'String','');
    else
        set(handles.thresh,'String',num2str(s));
        set(handles.thresh,'Value',s);
    end
end
guidata(hObject, handles)
function thresh_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function DSFact_Callback(hObject, eventdata, handles)
s=get(handles.DSFact,'String');
if strcmpi(s,'') || isempty(s)
    set(handles.DSFact,'Value',0.5);
    set(handles.DSFact,'String',num2str(0.5));
else
    s = str2num(s);
    if s < 0
        warndlg('Invalid resampling factor')
        return
        set(handles.DSFact,'String','');
    else
        set(handles.DSFact,'String',num2str(s));
        set(handles.DSFact,'Value',s);
    end
end
guidata(hObject, handles)
function DSFact_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function SpatFilt_Callback(hObject, eventdata, handles)
s=get(handles.SpatFilt,'String');
if strcmpi(s,'') || isempty(s)
    set(handles.SpatFilt,'Value',2);
    set(handles.SpatFilt,'String',num2str(2));
else
    s = str2num(s);
    if s < 0
        warndlg('Invalid spatial filter')
        return
    else
        set(handles.SpatFilt,'String',num2str(s));
        set(handles.SpatFilt,'Value',s);
    end
end
guidata(hObject, handles)
function SpatFilt_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function hmcut_Callback(hObject, eventdata, handles)
s=get(handles.hmcut,'String');
if strcmpi(s,'') || isempty(s)
    set(handles.hmcut,'Value',1.5);
    set(handles.hmcut,'String',num2str(1.5));
else
    s = str2num(s);
    if s < 0
        warndlg('Invalid homomorphic filter')
        set(handles.hmcut,'String','');
        return
    else
        set(handles.hmcut,'String',num2str(s));
        set(handles.hmcut,'Value',s);
    end
end
guidata(hObject, handles)
function hmcut_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function hwrect_Callback(hObject, eventdata, handles)
s=get(handles.hwrect,'String');
if strcmpi(s,'') || isempty(s)
    set(handles.hwrect,'Value',1);
    set(handles.hwrect,'String',num2str(1));
else
    s = str2num(s);
    if s < 0
        warndlg('Invalid halfwave rectification')
        set(handles.hwrect,'String','');
        return
    else
        set(handles.hwrect,'String',num2str(s));
        set(handles.hwrect,'Value',s);
    end
end
guidata(hObject, handles)
function hwrect_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function keyword_Callback(hObject, eventdata, handles)
function keyword_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
%Saving image figures
function savesurf_Callback(hObject, eventdata, handles)
[FileName,PathName] = uiputfile([handles.surfaceFilePath '*.png'],'Save Image');
if isstr(FileName)
    export_fig(handles.surfacefig,[PathName FileName],'-png')
end
function savedeltaF_Callback(hObject, eventdata, handles)
[FileName,PathName] = uiputfile([handles.surfaceFilePath '*.png'],'Save Image');
if isstr(FileName)
    export_fig(handles.deltaFfig,[PathName FileName],'-png')
end
function savemap_Callback(hObject, eventdata, handles)
[FileName,PathName] = uiputfile([handles.surfaceFilePath '*.png'],'Save Image');
if isstr(FileName)
    export_fig(handles.tonotopyfig,[PathName FileName],'-png')
end
function saveanalysis_Callback(hObject, eventdata, handles)
[FileName,PathName] = uiputfile([handles.surfaceFilePath '*.png'],'Save Image');
if isstr(FileName)
    export_fig(handles.analysisfig,[PathName FileName],'-png')
end
function magw_Callback(hObject, eventdata, handles)
