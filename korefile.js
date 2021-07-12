let project = new Project('littleKoreEngine');

project.addFile('Sources/**');
project.setDebugDir('Deployment');

resolve(project);