function(target_link_runtime target_name project_binary_directory)
    message(STATUS "Copy Immortal Runtime for target - ${target_name}")
    file(COPY ${IMMORTAL_ASSET_DIR} DESTINATION ${project_binary_directory})
    file(COPY ${IMMORTAL_RUNTIME} DESTINATION ${project_binary_directory})
endfunction()
