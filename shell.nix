{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = [
    pkgs.pkg-config

    pkgs.wayland
    pkgs.wayland-protocols
    pkgs.libxkbcommon

    pkgs.mesa
    pkgs.libglvnd
    pkgs.vulkan-loader
    pkgs.vulkan-tools
    pkgs.libdrm
    pkgs.egl-wayland

    pkgs.alsa-lib
  ];

  shellHook = ''
    export WINIT_UNIX_BACKEND=wayland

    export LD_LIBRARY_PATH=${
      pkgs.lib.makeLibraryPath [
        pkgs.wayland
        pkgs.libxkbcommon
        pkgs.mesa
        pkgs.libglvnd
        pkgs.vulkan-loader
        pkgs.libdrm
      ]
    }:$LD_LIBRARY_PATH
  '';
}
