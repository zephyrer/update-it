<?xml version="1.0"?>

<!--

UpdateIt! application.
Copyright (c) 2002-2009 by Elijah Zarezky,
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

CreateDoc.build.inc - 'create-doc' target

-->

<project>

	<target
		name ="create-doc"
		description="Create CHM documentation."
		depends="clean-doc">
		<exec
			program="hhc.exe"
			commandline="UpdateIt.0409.hhp"
			workingdir="${UpdateItPath}\HTML"
			resultproperty="HhcResult"
			failonerror="false">
		</exec>
		<echo/>
		<echo message="If return code was 1 it's not a failure but success!"/>
		<if test="${HhcResult != '1'}">
			<fail message="UpdateIt.0409.chm compilation failed."/>
		</if>
		<exec
			program="hhc.exe"
			commandline="UpdateIt.0419.hhp"
			workingdir="${UpdateItPath}\HTML"
			resultproperty="HhcResult"
			failonerror="false">
		</exec>
		<echo/>
		<echo message="If return code was 1 it's not a failure but success!"/>
		<if test="${HhcResult != '1'}">
			<fail message="UpdateIt.0419.chm compilation failed."/>
		</if>
	</target>

</project>
