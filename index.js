const lib = require('./build/Release/message-height-twitch.node');

console.log('Initial load success:', lib.InitializeLibrary());
console.log('Channel load success:', lib.InitializeChannel('simon36', '230654310', 5000));

console.log(
  'Message result:',
  lib.CalculateHeight('simon36', 'vadiChad', 'tetyys', 'TETYYS', 1, [{ Name: 'vadiChad', Url: 'https://static-cdn.jtvnw.net/emoticons/v1/302394764/1.0' }])
);
