<?xml version="1.0"?>

<!--

UpdateIt! application.
Copyright (c) 2002-2011 by Elijah Zarezky,
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

		<!-- UpdateIt.exe resources -->

		<copy
			file="${UpdateItPath}\Templates\Manifest.2008.xml"
			tofile="${UpdateItPath}\Source\Manifest.2008.xml"
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

		<copy
			file="${UpdateItPath}\Templates\VersionInfo.rc"
			tofile="${UpdateItPath}\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_COMMENTS" value="${VerComments}"/>
					<token key="APP_NAME" value="${AppName}"/>
					<token key="EXE_NAME" value="${ExeName}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<touch
			file="${UpdateItPath}\Source\UpdateIt.rc"
			datetime="${datetime::now()}"/>

		<!-- CrashReporter.exe resources -->

		<copy
			file="${UpdateItPath}\CrashReporter\Templates\Manifest.2008.xml"
			tofile="${UpdateItPath}\CrashReporter\Source\Manifest.2008.xml"
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

		<copy
			file="${UpdateItPath}\CrashReporter\Templates\VersionInfo.rc"
			tofile="${UpdateItPath}\CrashReporter\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_COMMENTS" value="${VerComments}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<touch
			file="${UpdateItPath}\CrashReporter\Source\CrashReporter.rc"
			datetime="${datetime::now()}"/>

		<!-- ZipArchiver.dll resources -->

		<copy
			file="ZipArchiver\Templates\VersionInfo.rc"
			tofile="ZipArchiver\Source\VersionInfo.rc"
			overwrite="true"
			if="${AppName == 'UpdateIt! Pro'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_COMMENTS" value="${VerComments}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<touch
			file="ZipArchiver\Source\ZipArchiver.rc"
			datetime="${datetime::now()}"
			if="${AppName == 'UpdateIt! Pro'}"/>

		<!-- English_USA.1252.dll resources -->

		<copy
			file="${UpdateItPath}\Languages\English_USA.1252\Templates\English_USA.1252.rc"
			tofile="${UpdateItPath}\Languages\English_USA.1252\Source\English_USA.1252.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="APP_NAME" value="${AppName}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<copy
			file="${UpdateItPath}\Languages\English_USA.1252\Templates\VersionInfo.rc"
			tofile="${UpdateItPath}\Languages\English_USA.1252\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_COMMENTS" value="${VerComments}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<touch
			file="${UpdateItPath}\Languages\English_USA.1252\Source\English_USA.1252.rc"
			datetime="${datetime::now()}"/>

		<!-- Russian_Russia.1251.dll resources -->

		<copy
			file="${UpdateItPath}\Languages\Russian_Russia.1251\Templates\Russian_Russia.1251.rc"
			tofile="${UpdateItPath}\Languages\Russian_Russia.1251\Source\Russian_Russia.1251.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="APP_NAME" value="${AppName}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<copy
			file="${UpdateItPath}\Languages\Russian_Russia.1251\Templates\VersionInfo.rc"
			tofile="${UpdateItPath}\Languages\Russian_Russia.1251\Source\VersionInfo.rc"
			overwrite="true">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_COMMENTS" value="${VerComments}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<touch
			file="${UpdateItPath}\Languages\Russian_Russia.1251\Source\Russian_Russia.1251.rc"
			datetime="${datetime::now()}"/>

		<!-- Inno Setup scripts -->

		<copy
			file="${UpdateItPath}\Templates\Setup.iss"
			tofile="${UpdateItPath}\Source\Setup.iss"
			overwrite="true"
			if="${AppName == 'UpdateIt!'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_SUFFIX" value="${VerSuffix}"/>
					<token key="CHARACTER_SET" value="${CharacterSet}"/>
					<token key="BUILD_TYPE" value="${BuildType}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<copy
			file="${UpdateItPath}\Templates\SetupUniversal.iss"
			tofile="${UpdateItPath}\Source\SetupUniversal.iss"
			overwrite="true"
			if="${AppName == 'UpdateIt!'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_SUFFIX" value="${VerSuffix}"/>
					<token key="CHARACTER_SET" value="${CharacterSet}"/>
					<token key="BUILD_TYPE" value="${BuildType}"/>
				</replacetokens>
			</filterchain>
		</copy>

		<copy
			file="UpdateItPro\Templates\SetupPro.iss"
			tofile="UpdateItPro\Source\SetupPro.iss"
			overwrite="true"
			if="${AppName == 'UpdateIt! Pro'}">
			<filterchain>
				<replacetokens>
					<token key="VER_MAJOR" value="${VerMajor}"/>
					<token key="VER_MINOR" value="${VerMinor}"/>
					<token key="VER_BUILD" value="${VerBuild}"/>
					<token key="SVN_REV" value="${SvnRev}"/>
					<token key="VER_SUFFIX" value="${VerSuffix}"/>
					<token key="CHARACTER_SET" value="${CharacterSet}"/>
					<token key="BUILD_TYPE" value="${BuildType}"/>
				</replacetokens>
			</filterchain>
		</copy>

	</target>

</project>
