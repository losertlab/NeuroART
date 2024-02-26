﻿<?xml version="1.0" encoding="utf-8"?>
<SourceFile Checksum="224363A73AEF722D8A2FADC1491C5B675CD2BF21BB64EFAD31B01566E20AC2F4A48C177EF3D3CC6527415B1DCFF66043AD949641B3EEF610D67030EA3C728E95" Timestamp="1D574FE3BCDCB6F" xmlns="http://www.ni.com/PlatformFramework">
	<SourceModelFeatureSet>
		<ParsableNamespace AssemblyFileVersion="6.4.0.49504" FeatureSetName="Interface for MATLAB®" Name="http://www.ni.com/Matlab" OldestCompatibleVersion="5.3.1.49152" Version="5.3.1.49152" />
		<ParsableNamespace AssemblyFileVersion="6.4.0.49504" FeatureSetName="Editor" Name="http://www.ni.com/PlatformFramework" OldestCompatibleVersion="6.2.0.49154" Version="6.2.0.49154" />
		<ApplicationVersionInfo Build="6.4.0.49504" Name="LabVIEW NXG" Version="3.1.0" />
	</SourceModelFeatureSet>
	<MatlabDefinition Id="31f1eeff8e194bf88cc493f93cb6a123" xmlns="http://www.ni.com/Matlab">
		<p.PlatformAgnosticPathToLibrary>
			<Path a="0">
				<pE>otslm.simple.linear</pE>
			</Path>
		</p.PlatformAgnosticPathToLibrary>
		<Icon Id="c2ac3c60affe43cea4579fa9ba639380" ListViewIconCrop="0 0 40 40" xmlns="http://www.ni.com/PlatformFramework">
			<IconPanel Height="[float]40" Id="a7978bcc75c548debdf7b7aaefbe8b64" Left="[float]0" MinHeight="[float]0" MinWidth="[float]0" PanelSizeMode="Resize" Top="[float]0" Width="[float]40">
				<IconTemplate Height="[float]40" Id="e3982246ce1340e8b5d49c73b15379c6" Left="[float]0" TemplateName="[string]Blank" Top="[float]0" Width="[float]40" />
			</IconPanel>
		</Icon>
	</MatlabDefinition>
	<EnvoyManagerFile Id="229c2fc4e0b045ecb2cbb9cd6b7adc83" xmlns="http://www.ni.com/PlatformFramework">
		<ProjectSettings Id="6fecf213f99b480b8b5e5b6d930c2d85" ModelDefinitionType="ProjectSettings" Name="ZProjectSettings" />
		<NameScopingEnvoy AutomaticallyResolveUp="True" Id="ab6d5c6be934b92bb8131efa67e98ba" Name="linear.mli" NameTracksFileName="True">
			<EmbeddedDefinitionReference Id="247b1c58b81a4b0f82e619a0925fbd8a" ModelDefinitionType="{http://www.ni.com/Matlab}MatlabEntryPoint" Name="otslm.simple.linear">
				<MatlabEntryPoint Id="9f5913610ee84e79b43ac1ca69de6828" xmlns="http://www.ni.com/Matlab">
					<Icon Id="be4d5e2f7b404270a34c0078b8135f6f" ListViewIconCrop="0 0 40 60" xmlns="http://www.ni.com/PlatformFramework">
						<IconPanel Height="[float]60" Id="b09c9060cf69437fb6d5a9b78dd96851" Left="[float]0" MinHeight="[float]0" MinWidth="[float]0" PanelSizeMode="Resize" Top="[float]0" Width="[float]40">
							<IconTemplate ClipMargin="[SMThickness]3,3,3,3" Height="[float]60" Id="d38ed4ca29c840f398b671d68014d47d" Left="[float]0" TemplateName="[string]Gray" Top="[float]0" Width="[float]40">
								<Rectangle Fill="[SMSolidColorBrush]#ff727272" Id="8bf6373cad384ace9c43ca31ec642545" IsHitTestVisible="[bool]False" Left="[float]0" MinHeight="[float]1" MinWidth="[float]1" RadiusX="[float]4" RadiusY="[float]4" Top="[float]0" />
								<Rectangle Fill="[SMSolidColorBrush]#ffe5e5e5" Id="416e2384e7874cfaa00645e94a3dda65" IsHitTestVisible="[bool]False" Left="[float]0" Margin="[SMThickness]1,1,1,1" MinHeight="[float]1" MinWidth="[float]1" RadiusX="[float]2.5" RadiusY="[float]2.5" Stroke="[SMSolidColorBrush]#fff2f2f2" Top="[float]0" />
								<FileNameText Attached="[bool]True" Height="[float]56" Id="d6eb0f821e6449fbacaba5ec4cd4d899" Left="[float]0" Margin="[SMThickness]2,2,2,2" SizeMode="[TextModelSizeMode]AutoFont" Text="[string]otslm.simple" TextAlignment="[TextAlignment]Center" TextWrapping="[TextWrapping]Wrap" Top="[float]0" VerticalScrollBarVisibility="[ScrollBarVisibility]Hidden" Width="[float]36">
									<FontSetting FontFamily="Verdana" FontSize="6" Id="b2bf9eed108e4e5587085d3d08a0bcf3" />
								</FileNameText>
							</IconTemplate>
						</IconPanel>
					</Icon>
					<ConnectorPane Height="60" Id="36aad8f6379e496cb4a395825ac4d7c4" ListViewHeight="130" ListViewWidth="150" Width="40" xmlns="http://www.ni.com/PlatformFramework">
						<ConnectorPaneTerminal />
						<ConnectorPaneTerminal Hotspot="0 15" ListViewHotspot="0 20" Parameter="df7ed5cee6c942beac14620338247dc6" />
						<ConnectorPaneTerminal Hotspot="0 25" ListViewHotspot="0 35" Parameter="100f7359c6974f5a8a35ee0d82e2cdcb" />
						<ConnectorPaneTerminal Hotspot="0 35" ListViewHotspot="0 5" Parameter="40675ed12e2e482bb12bce0299c94465" />
						<ConnectorPaneTerminal Hotspot="15 0" />
						<ConnectorPaneTerminal Hotspot="25 0" />
						<ConnectorPaneTerminal Hotspot="40 5" ListViewHotspot="150 50" Parameter="f433d397341f43f78ff6563acafbe75a" />
						<ConnectorPaneTerminal Hotspot="40 15" />
						<ConnectorPaneTerminal Hotspot="40 25" />
						<ConnectorPaneTerminal Hotspot="40 35" />
						<ConnectorPaneTerminal Hotspot="15 60" />
						<ConnectorPaneTerminal Hotspot="25 60" />
						<ConnectorPaneTerminal Hotspot="0 45" ListViewHotspot="0 65" Parameter="c456a7e761a442f1afe00da0506b75cd" />
						<ConnectorPaneTerminal Hotspot="40 45" />
						<ConnectorPaneTerminal Hotspot="0 55" ListViewHotspot="0 80" Parameter="ddba2e3578d1424cadae7d7514a5c035" />
						<ConnectorPaneTerminal Hotspot="40 55" ListViewHotspot="150 95" Parameter="6bec0f917afd49b18597c7826d400da1" />
					</ConnectorPane>
					<MatlabErrorDiagramParameter CallDirection="Input" Id="ddba2e3578d1424cadae7d7514a5c035" Name="error in" />
					<MatlabErrorDiagramParameter CallDirection="Output" Id="6bec0f917afd49b18597c7826d400da1" Name="error out" />
					<MatlabParameter DataType="Double[,]" Id="16612e4c67b0485eaa7e3511385e759a" Name="im">
						<MatlabDiagramParameter CallDirection="Input" Id="8541c6e038134a34a0e0819598f8a502" Name="im in" Visible="False" />
						<MatlabDiagramParameter CallDirection="Output" Id="f433d397341f43f78ff6563acafbe75a" Name="im" />
					</MatlabParameter>
					<MatlabParameter DataType="Double[]" Id="cd70ffb1d17643d9b52005b847c14093" Name="sz">
						<MatlabDiagramParameter CallDirection="Input" Id="df7ed5cee6c942beac14620338247dc6" Name="sz" />
						<MatlabDiagramParameter CallDirection="Output" Id="6f3d88e60f10434ba1d0a5814242953c" Name="sz out" Visible="False" />
					</MatlabParameter>
					<MatlabParameter DataType="Double[]" Id="29455fe5156141c088d669ac91508acd" Name="spacing">
						<MatlabDiagramParameter CallDirection="Input" Id="100f7359c6974f5a8a35ee0d82e2cdcb" Name="spacing" />
						<MatlabDiagramParameter CallDirection="Output" Id="890fe861da18495ab10f546a8cbed623" Name="spacing out" Visible="False" />
					</MatlabParameter>
					<MatlabParameter DataType="String" Id="cdd78cbd82bc4af3b5d6beeaf663ee16" Name="centre_str">
						<MatlabDiagramParameter CallDirection="Input" Id="40675ed12e2e482bb12bce0299c94465" Name="centre_str" />
						<MatlabDiagramParameter CallDirection="Output" Id="61b086f2ba914df7a3094ff42fa3cd7a" Name="centre_str out" Visible="False" />
					</MatlabParameter>
					<MatlabParameter DataType="Double[]" Id="e4d87b9b35c949f2b355fa65b24a896c" Name="centre">
						<MatlabDiagramParameter CallDirection="Input" Id="c456a7e761a442f1afe00da0506b75cd" Name="centre" />
						<MatlabDiagramParameter CallDirection="Output" Id="d0c2333d76b3432e819b02a27a4489a0" Name="centre out" Visible="False" />
					</MatlabParameter>
				</MatlabEntryPoint>
			</EmbeddedDefinitionReference>
		</NameScopingEnvoy>
	</EnvoyManagerFile>
</SourceFile>