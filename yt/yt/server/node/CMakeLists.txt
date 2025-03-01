
# This file was generated by the build system used internally in the Yandex monorepo.
# Only simple modifications are allowed (adding source-files to targets, adding simple properties
# like target_include_directories). These modifications will be ported to original
# ya.make files by maintainers. Any complex modifications which can't be ported back to the
# original buildsystem will not be accepted.


add_subdirectory(cellar_node)
add_subdirectory(chaos_node)
add_subdirectory(data_node)
add_subdirectory(query_agent)
add_subdirectory(tablet_node)
get_built_tool_path(
  TOOL_protoc_bin
  TOOL_protoc_dependency
  contrib/tools/protoc/bin
  protoc
)
get_built_tool_path(
  TOOL_cpp_styleguide_bin
  TOOL_cpp_styleguide_dependency
  contrib/tools/protoc/plugins/cpp_styleguide
  cpp_styleguide
)
get_built_tool_path(
  TOOL_protoc_bin
  TOOL_protoc_dependency
  contrib/tools/protoc/bin
  protoc
)
get_built_tool_path(
  TOOL_cpp_styleguide_bin
  TOOL_cpp_styleguide_dependency
  contrib/tools/protoc/plugins/cpp_styleguide
  cpp_styleguide
)
get_built_tool_path(
  TOOL_protoc_bin
  TOOL_protoc_dependency
  contrib/tools/protoc/bin
  protoc
)
get_built_tool_path(
  TOOL_cpp_styleguide_bin
  TOOL_cpp_styleguide_dependency
  contrib/tools/protoc/plugins/cpp_styleguide
  cpp_styleguide
)
get_built_tool_path(
  TOOL_protoc_bin
  TOOL_protoc_dependency
  contrib/tools/protoc/bin
  protoc
)
get_built_tool_path(
  TOOL_cpp_styleguide_bin
  TOOL_cpp_styleguide_dependency
  contrib/tools/protoc/plugins/cpp_styleguide
  cpp_styleguide
)

add_library(yt-server-node)
target_compile_options(yt-server-node PRIVATE
  -Wdeprecated-this-capture
)
target_include_directories(yt-server-node PUBLIC
  ${CMAKE_BINARY_DIR}/yt
)
target_include_directories(yt-server-node PRIVATE
  ${CMAKE_SOURCE_DIR}/contrib/libs/sparsehash/src
)
target_link_libraries(yt-server-node PUBLIC
  contrib-libs-linux-headers
  contrib-libs-cxxsupp
  yutil
  library-query-engine
  library-query-row_comparer
  library-dns_over_rpc-server
  yt-library-re2
  yt-library-containers
  library-containers-cri
  library-containers-disk_manager
  yt-library-gpu
  yt-ytlib-distributed_throttler
  server-node-cellar_node
  server-node-chaos_node
  yt-server-tools
  ytserver
  server-lib-io
  server-lib-cellar_agent
  server-lib-chaos_node
  server-lib-chunk_server
  server-lib-exec_node
  server-lib-tablet_server
  server-lib-hydra
  lib-hydra-dry_run
  server-lib-lsm
  server-lib-nbd
  server-lib-rpc
  server-lib-controller_agent
  library-cpp-getopt
  cpp-yt-phdr_cache
  libs-tcmalloc-malloc_extension
  contrib-libs-protobuf
)
target_proto_messages(yt-server-node PRIVATE
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/artifact.proto
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/volume.proto
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/in_memory_service.proto
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/transaction_manager.proto
)
target_sources(yt-server-node PRIVATE
  ${CMAKE_BINARY_DIR}/yt/yt/server/node/data_node/artifact.pb.h
  ${CMAKE_BINARY_DIR}/yt/yt/server/node/exec_node/volume.pb.h
  ${CMAKE_BINARY_DIR}/yt/yt/server/node/tablet_node/in_memory_service.pb.h
  ${CMAKE_BINARY_DIR}/yt/yt/server/node/tablet_node/transaction_manager.pb.h
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/cluster_node/proxying_chunk_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/cluster_node/dynamic_config_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/cluster_node/node_resource_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/cluster_node/master_connector.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/cluster_node/bootstrap.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/cluster_node/config.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/ally_replica_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/artifact.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/blob_chunk.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/blob_reader_cache.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/blob_session.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/bootstrap.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/chunk.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/chunk_detail.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/chunk_meta_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/chunk_registry.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/chunk_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/chunk_reader_sweeper.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/config.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/data_node_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/disk_location.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/job.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/job_controller.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/job_info.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/journal_chunk.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/journal_dispatcher.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/journal_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/journal_session.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/io_throughput_meter.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/local_chunk_reader.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/location.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/location_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/master_connector.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/medium_directory_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/medium_updater.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/network_statistics.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/offloaded_chunk_read_session.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/orchid.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/p2p.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/session_detail.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/session_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/skynet_http_handler.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/table_schema_cache.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/data_node/ytree_integration.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/bootstrap.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/controller_agent_connector.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/chunk_cache.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/cache_location.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/exec_node_admin_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/helpers.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/gpu_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_environment.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_info.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_directory_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_gpu_checker.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_controller.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_prober_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/job_workspace_builder.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/master_connector.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/orchid.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/public.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/scheduler_connector.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/slot.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/slot_location.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/slot_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/supervisor_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/exec_node/volume_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/job_agent/job_resource_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/config.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/helpers.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/multiread_request_queue_provider.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/query_executor.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/query_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/replication_log_batch_reader.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/session.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/query_agent/session_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/automaton.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/background_activity_orchid.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/backing_store_cleaner.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/backup_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/bootstrap.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/cached_row.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/chaos_agent.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/chunk_view_size_fetcher.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/compaction_hint_fetcher.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/compression_dictionary_builder.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/compression_dictionary_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/distributed_throttler_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/dynamic_store_bits.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hedging_manager_registry.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hint_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_chunk.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_chunk_sweeper.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_lock_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_tablet.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_tablet_scanner.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunk_tablet_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/hunks_serialization.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/in_memory_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/in_memory_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/lock_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/locking_state.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/lookup.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/lsm_interop.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/master_connector.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/mutation_forwarder.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/mutation_forwarder_thunk.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/object_detail.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/ordered_chunk_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/ordered_dynamic_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/ordered_store_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/partition.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/partition_balancer.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/public.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/relative_replication_throttler.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/replicated_store_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/replication_log.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/row_cache.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/row_digest_fetcher.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/security_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/serialize.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/slot_provider.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/slot_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/smooth_movement_tracker.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/sorted_chunk_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/sorted_dynamic_comparer.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/sorted_dynamic_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/sorted_store_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/statistics_reporter.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store_compactor.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store_detail.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store_flusher.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store_manager_detail.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store_rotator.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/store_trimmer.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/structured_logger.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/table_config_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/table_replicator.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/table_puller.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_cell_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_cell_snapshot_validator.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_memory_statistics.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_cell_write_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_profiling.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_reader.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_service.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_slot.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_snapshot_store.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/tablet_write_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/transaction.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/transaction_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/versioned_chunk_meta_manager.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/overload_controller.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/server/node/tablet_node/overload_controlling_service_base.cpp
)
target_proto_addincls(yt-server-node
  ./yt
  ${CMAKE_SOURCE_DIR}/yt
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/yt
  ${CMAKE_SOURCE_DIR}/yt
  ${CMAKE_SOURCE_DIR}/contrib/libs/protobuf/src
  ${CMAKE_SOURCE_DIR}/yt/yt/contrib/cri-api
  ${CMAKE_SOURCE_DIR}/yt/yt/contrib/gogoproto
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/contrib/libs/protobuf/src
)
target_proto_outs(yt-server-node
  --cpp_out=${CMAKE_BINARY_DIR}/yt
  --cpp_styleguide_out=${CMAKE_BINARY_DIR}/yt
)
