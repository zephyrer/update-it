<?xml version="1.0"?>

<!--

UpdateIt! application.
Copyright (c) 2002-2007 by Elijah Zarezky,
All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

StampVer.build.inc - 'stamp-ver' target

-->

<project>

	<target
		name="stamp-ver"
		description="Stamp version numbers and names."
		depends="verify-ver">
		
		<echo message="Writing version numbers and names..."/>
		<echo/>

		<!-- UpdateIt.exe manifest -->

		<copy
			file="${UpdateItPath}\Source\UpdateIt.manifest.nant"
			tofile="${UpdateItPath}\Source\UpdateIt.manifest"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="APP_NAME" value="${AppName}"/>
					<token key="EXE_NAME" value="${ExeName}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<!-- UpdateIt.exe VERSIONINFO -->

		<copy
			file="${UpdateItPath}\Source\VersionInfo.rc.nant"
			tofile="${UpdateItPath}\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="APP_NAME" value="${AppName}"/>
					<token key="EXE_NAME" value="${ExeName}"/>
				</replacetokens>
			</filterchain>
		</copy>
		
		<touch
			file="${UpdateItPath}\Source\UpdateIt.rc"
			datetime="${datetime::now()}"/>
		
		<!-- ZipArchiver.dll VERSIONINFO -->

		<copy
			file="ZipArchiver\Source\VersionInfo.rc.nant"
			tofile="ZipArchiver\Source\VersionInfo.rc"
			overwrite="true"
			if="${AppName == 'UpdateIt! Pro'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
				</replacetokens>
			</filterchain>
		</copy>
		
		<touch
			file="ZipArchiver\Source\ZipArchiver.rc"
			datetime="${datetime::now()}"
			if="${AppName == 'UpdateIt! Pro'}"/>
		
		<!-- English_USA.1252.exe VERSIONINFO -->

		<copy
			file="${UpdateItPath}\Languages\English_USA.1252\Source\VersionInfo.rc.nant"
			tofile="${UpdateItPath}\Languages\English_USA.1252\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
				</replacetokens>
			</filterchain>
		</copy>
		
		<touch
			file="${UpdateItPath}\Languages\English_USA.1252\Source\English_USA.1252.rc"
			datetime="${datetime::now()}"/>
		
		<!-- Russian_Russia.1251.exe VERSIONINFO -->

		<copy
			file="${UpdateItPath}\Languages\Russian_Russia.1251\Source\VersionInfo.rc.nant"
			tofile="${UpdateItPath}\Languages\Russian_Russia.1251\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
				</replacetokens>
			</filterchain>
		</copy>
		
		<touch
			file="${UpdateItPath}\Languages\Russian_Russia.1251\Source\Russian_Russia.1251.rc"
			datetime="${datetime::now()}"/>
		
		<!-- Inno Setup scripts -->

		<copy
			file="${UpdateItPath}\Source\Setup.iss.nant"
			tofile="${UpdateItPath}\Source\Setup.iss"
			overwrite="true"
			if="${AppName == 'UpdateIt!'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
				</replacetokens>
			</filterchain>
		</copy>
		
		<copy
			file="${UpdateItPath}\Source\SetupUniversal.iss.nant"
			tofile="${UpdateItPath}\Source\SetupUniversal.iss"
			overwrite="true"
			if="${AppName == 'UpdateIt!'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
				</replacetokens>
			</filterchain>
		</copy>
		
		<copy
			file="UpdateItPro\Source\SetupPro.iss.nant"
			tofile="UpdateItPro\Source\SetupPro.iss"
			overwrite="true"
			if="${AppName == 'UpdateIt! Pro'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
				</replacetokens>
			</filterchain>
		</copy>
	
	</target>

</project>
