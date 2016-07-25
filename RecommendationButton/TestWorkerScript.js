
WorkerScript.onMessage = function(m) {
    WorkerScript.sendMessage({ 'getSize': (m.GW - (m.GS * 2) - m.PL - m.PR) / 3})
}
