{
  description = "Entorno de Programacion UNLaM - C/C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        # Paquetes necesarios para las materias de programación
        buildInputs = with pkgs; [
          # Compilador y herramientas de build
          gcc
          gdb
          gnumake
          binutils
	  clang
	  doxygen

          
          # El IDE que mencionaste
          codeblocks
          
          # Extra: Valgrind es vital en la UNLaM para ver fugas de memoria
          valgrind
        ];

        # Configuración para que Code::Blocks encuentre las cabeceras de C
        shellHook = ''
          export CPATH="${pkgs.glibc.dev}/include"
          echo "--- Entorno UNLaM cargado ---"
          echo "Compilador: $(gcc --version | head -n 1)"
          echo "Escribe 'codeblocks' para lanzar el IDE"
        '';
      };
    };
}
