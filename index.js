const lib = require('./build/Release/message-height-twitch.node');

console.log('Initial load success:', lib.initializeLibrary());
console.log('Channel load success:', lib.initializeChannel('simon36', '230654310', 5000));

console.log(
  'Message result:',
  lib.calculateHeight('simon36', 'it works now BOP 💀', 'tetyys', 'TETYYS', 1, [{ name: 'BOP', url: 'https://static-cdn.jtvnw.net/emoticons/v1/301428702/1.0' }])
);
