const lib = require('./build/Release/message-height-twitch.node');

console.log('Initial load success:', lib.InitializeLibrary());
console.log('Channel load success:', lib.InitializeChannel('simon36', '230654310', 5000));
