declare module 'nrfdfu-node'
{
    export function uploadFirmware(address: string, filePath: string,
                                   callback: (result: number) => void): void
}
