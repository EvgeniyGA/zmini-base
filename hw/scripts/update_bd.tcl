source ../scripts/settings.tcl

open_project ${project_name}.xpr
set work_dir [get_property DIRECTORY [current_project]]
open_bd_design [get_files ${work_dir}/${project_name}.srcs/sources_1/bd/bd/bd.bd]
write_bd_tcl [get_property DIRECTORY [current_project]]/../source/scripts/bd.tcl -include_layout -force

