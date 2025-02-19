{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
    ...
  } @ inputs: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};

    venvDir = "venv";
    pythonPackages = pkgs.python314Packages;
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        bear
        avrdude
        platformio

        pythonPackages.python
      ];

      shellHook = ''
        export PYTHONPATH=$PWD/${venvDir}/${pythonPackages.python.sitePackages}/:$PYTHONPATH
        export TMPDIR=$HOME/.cache/pip
        export VENV_DIR=${venvDir}

        if [ -d "${venvDir}" ]; then
          echo "Skipping venv creation, '${venvDir}' already exists"
        else
          ${pythonPackages.python.interpreter} -m venv "${venvDir}"
        fi

        source "${venvDir}/bin/activate"
      '';
    };
  };
}
