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

CleanDoc.build.inc - 'clean-doc' target

-->

<project>

	<target
		name="clean-doc"
		description="Delete CHM documentation.">
		<delete
			file="${UpdateItPath}\HTML\UpdateIt.0409.chm"
			if="${file::exists(path::combine(UpdateItPath, 'HTML\UpdateIt.0409.chm'))}"/>
		<delete
			file="${UpdateItPath}\HTML\UpdateIt.0419.chm"
			if="${file::exists(path::combine(UpdateItPath, 'HTML\UpdateIt.0419.chm'))}"/>
	</target>

</project>
