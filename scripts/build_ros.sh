# scripts/build_ros.sh
#!/usr/bin/env bash
set -euo pipefail

if [[ -n "${CONDA_PREFIX:-}" ]]; then
  echo "Error: Conda env is active: $CONDA_PREFIX"
  echo "Run: conda deactivate"
  exit 1
fi

source /opt/ros/jazzy/setup.bash
colcon build --symlink-install