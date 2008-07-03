<?xml version="1.0"?>

<!--

UpdateIt! application.
Copyright (c) 2002-2008 by Elijah Zarezky,
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

VerifyVer.build.inc - 'verify-ver' target

-->

<project>

	<target
		name="verify-ver"
		description="Verify version numbers and names.">
		<echo message="Verifying version numbers and names..."/>
		<echo/>
		<if test="${VerMajor == '0'}">
			<fail message="Major version number isn't specified."/>
		</if>
		<if test="${VerMinor == '0'}">
			<fail message="Minor version number isn't specified."/>
		</if>
		<if test="${VerBuild == '0'}">
			<fail message="Build number isn't specified."/>
		</if>
		<if test="${SvnRev == '0'}">
			<fail message="Revision number isn't specified."/>
		</if>
		<echo message="Version is: ${VerMajor}.${VerMinor}.${VerBuild}.${SvnRev}"/>
	</target>

</project>
