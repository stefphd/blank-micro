function gencode(modelname,destdir)
%GENCODE(input) generates the C/C++ code a Simulink model using the Embedeed Coder.
%Created by Stefano Lovato
%Creation data: 6th April 2022
%Last edit: 6th Aprile 2022
%Created with MATLAB R2022a
%Usages:
%   - gencode()                         Generate code in the directory './lib/modelname' for the first *.slx file found in the current directory (if any).
%   - gencode(modelname)                Generate code in the directory './lib/modelname' for the Simulink model 'modelname'.
%   - check_toolbox('~', destdir)       Generate code in the directory './destdir/modelname' for the first *.slx file found in the current directory (if any).
%   - check_toolbox(modelname,destdir)  Generate code in the directory './destdir/modelname' for the Simulink model 'modelname'.

fprintf('###################################################\n');
fprintf('# Generate code\n');
fprintf('# Created by Stefano Lovato\n');
fprintf('# Creation data: 5th April 2022\n');
fprintf('# Last edit: 6th Aprile 2022\n');
fprintf('# Created with MATLAB R2022a\n');
fprintf('# Type ''gencode --help'' or ''gencode -h'' for help.\n')
fprintf('###################################################\n');

%% Validate input
if nargin>0
    if not(ischar(modelname))
        error('''modelname'' must be a string.')
    end
end
if nargin>1
    if not(ischar(destdir))
        error('''destdir'' must be a string.')
    end
end

%% Check help
if nargin>0
    input = modelname;
    if ischar(input)
        if strcmp(input, '--help') || strcmp(input, '-h')
            fprintf('# Help\n')
            fprintf('# Description: generate the C/C++ code a Simulink model using the Embedeed Coder.\n');
            fprintf('# Usages:\n');
            fprintf('#\t- gencode()                         Generate code in the directory ''./lib/modelname'' for the first *.slx file found in the current directory (if any).\n');
            fprintf('#\t- gencode(modelname)                Generate code in the directory ''./lib/modelname'' for the Simulink model ''modelname''.\n');
            fprintf('#\t- check_toolbox(''~'', destdir)       Generate code in the directory ''./destdir/modelname'' for the first *.slx file found in the current directory (if any).\n');
            fprintf('#\t- check_toolbox(modelname,destdir)  Generate code in the directory ''./destdir/modelname'' for the Simulink model ''modelname''.\n');
            return;
        end
    end
end

%% gen code
if (nargin<1)
    modelname = '~';
end
if modelname == '~' %use default .slx file found
    %find first .slx file found
    files = dir('*.slx');
    if isempty(files)
        error('no *.slx files found in the current directory');
    end
    modelname = files(1).name(1:end-4);
end
if nargin<2
    destdir = 'lib';
end

if not(exist([modelname '.slx'], 'file'))
    error(['file ' modelname '.slx not found']);
end

CodeGenFolder = [destdir '/' modelname];

Simulink.fileGenControl('set', ...
    'CacheFolder', '.cache', ...
    'CodeGenFolder', CodeGenFolder, ...
    'CodeGenFolderStructure', ...
    Simulink.filegen.CodeGenFolderStructure.ModelSpecific, ...
    'createDir', true); %setup folders

load_system([modelname '.slx']); %load simulink model
set_param(modelname,'GenCodeOnly','on'); %set generate code only to on (do not generate .exe, useless)
if ispc %Windows - Automatically locate an installed toolchain not wokring b/c only for C, not C++
    set_param(modelname,'Toolchain','Microsoft Visual C++ 2017 v15.0 | nmake (64-bit Windows'); %Use MV C++
elseif isunix || ismac %Unix/Linux or Mac - use Automatically locate an installed toolchain
    set_param(modelname,'Toolchain','Automatically locate an installed toolchain'); %auto toolchain (should be gcc/g++)
else %something wrong
    error('OS not supported yet.');
end
slbuild(modelname) %build

Simulink.fileGenControl('reset') %reset setup

%% make src folder to collect all generated files
dir_main = [CodeGenFolder '/' modelname '_ert_rtw' '/'];
dir_shared = [CodeGenFolder '/slprj/ert/_sharedutils/'];
dir_codegen = [CodeGenFolder '/src/'];
codegen_file = [CodeGenFolder '/genlibs.h'];

list_h_main = dir([dir_main '*.h']);
list_c_main = dir([dir_main '*.c']);
list_cpp_main = dir([dir_main '*.cpp']);
list_h_shared = dir([dir_shared '*.h']);
list_c_shared = dir([dir_shared '*.c']);
list_cpp_shared = dir([dir_shared '*.cpp']);
fprintf('### Performing post-generation operations...\n');
fileID = fopen(codegen_file,'w');

if exist(dir_codegen,'dir') 
    rmdir(dir_codegen,'s')
end

if ~exist(dir_codegen,'dir') 
    mkdir(dir_codegen);
end

for k = 1 : numel(list_h_main) 
    if ~contains(list_h_main(k).name,'main') %exclude main files
        fprintf(fileID,'#include "src/%s"\n', list_h_main(k).name);
        copyfile([dir_main list_h_main(k).name], [dir_codegen list_h_main(k).name]);
    end
end
for k = 1 : numel(list_c_main)
    if ~contains(list_c_main(k).name,'main') %exclude main files
        copyfile([dir_main list_c_main(k).name], [dir_codegen list_c_main(k).name]);
    end
end
for k = 1 : numel(list_cpp_main) 
    if ~contains(list_cpp_main(k).name,'main') %exclude main files
        copyfile([dir_main list_cpp_main(k).name], [dir_codegen list_cpp_main(k).name]);
    end
end
for k = 1 : numel(list_h_shared) 
    if ~contains(list_h_shared(k).name,'main') %exclude main files;
        fprintf(fileID,'#include "src/%s"\n', list_h_shared(k).name);
        copyfile([dir_shared list_h_shared(k).name], [dir_codegen list_h_shared(k).name]);
    end
end
for k = 1 : numel(list_c_shared)
    if ~contains(list_c_shared(k).name,'main') %exclude main files
        copyfile([dir_shared list_c_shared(k).name], [dir_codegen list_c_shared(k).name]);
    end
end
for k = 1 : numel(list_cpp_shared) 
    if ~contains(list_cpp_shared(k).name,'main') %exclude main files
        copyfile([dir_shared list_cpp_shared(k).name], [dir_codegen list_cpp_shared(k).name]);
    end
end
fclose(fileID);

%% end
fprintf('Code generation finished\n');

%% EOF
fprintf('###################################################\n');

end
