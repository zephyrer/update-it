<?xml version="1.0"?>

<!--

UpdateIt! application.
Copyright (c) 2002-2010 by Elijah Zarezky,
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

SaveSymbols.build.inc - 'save-symbols' target

-->

<project>

	<target
		name="save-symbols"
		description="Save symbol files.">
		<property name="VersionStr" value="${VerMajor}.${VerMinor}.${VerBuild}.${SvnRev}"/>
		<property name="SymbolsDir" value="Symbols\${VersionStr}"/>
		
		<mkdir
			dir="${SymbolsDir}"
			if="${not directory::exists(SymbolsDir)}"/>
		
		<echo message="Saving symbol files."/>
		<echo/>
		
		<copy
			file="${OutputRootPath}\x86\${BuildType}\${CharacterSet}\${ExeName}.map"
			tofile="${SymbolsDir}\${ExeName}.map"
			overwrite="true">
		</copy>
		
		<copy
			file="${OutputRootPath}\x86\${BuildType}\${CharacterSet}\${ExeName}.pdb"
			tofile="${SymbolsDir}\${ExeName}.pdb"
			overwrite="true">
		</copy>

		<copy
			file="ZipArchiver\Output.2008\x86\${BuildType}\${CharacterSet}\ZipArchiver.map"
			tofile="${SymbolsDir}\ZipArchiver.map"
			overwrite="true"
			if="${AppName == 'UpdateIt! Pro'}">
		</copy>

		<copy
			file="ZipArchiver\Output.2008\x86\${BuildType}\${CharacterSet}\ZipArchiver.pdb"
			tofile="${SymbolsDir}\ZipArchiver.pdb"
			overwrite="true"
			if="${AppName == 'UpdateIt! Pro'}">
		</copy>

	</target>

</project>
