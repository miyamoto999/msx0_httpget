import http from "node:http";
import path from "node:path";
import fs from "node:fs";
import fsPromises from "fs/promises";
import mime from "mime";
import os, { NetworkInterfaceInfo } from "node:os";

const port = 9999;
const webdata = "./webdata";

async function responseFile(res:http.ServerResponse, filename:string):Promise<boolean>
{
	let ret = true;

	try {
		console.log("filename:" + filename);
		let st = await fsPromises.stat(filename);
		let filename2 = filename;
		if(st.isDirectory()) {
			filename2 = path.join(filename, "index.html");
			st = await fsPromises.stat(filename2);
		}
		console.log("filename2:" + filename2);
		const mimeType = mime.getType(filename2);
		console.log("mime:" + mimeType);
	
		if(mimeType) {
			res.setHeader("Content-Type", mimeType);
		}
		// res.setHeader("Content-Length", st.size);
	
		let stream = fs.createReadStream(filename2);
		stream.pipe(res);
	} catch(err) {
        console.log(err);
		ret = false;
	}

	return ret;
}

async function responseText(res:http.ServerResponse, statusCode:number, text:string, mimeType:string | null = "text/plain"):Promise<boolean> {
	let ret = true;

	try {
		if(mimeType) {
			res.setHeader("Content-Type", mimeType);
		}
		const buf = (new TextEncoder()).encode(text);
		res.setHeader("Content-Length", buf.length);
		res.statusCode = statusCode;
		res.end(buf);
	} catch(err) {
		console.log(err);
		ret = false;
	}
	return ret;
}

async function response404(res:http.ServerResponse):Promise<boolean> {
	return await responseText(res, 404, "404 Not Found.\n");
}

const server = http.createServer(async (req, res) => {
    let filepath = "";
            
    try {
        const u = new URL(req.url as string, "http://localhost");

        console.log("url:" + u.pathname);

        filepath = path.join(webdata, u.pathname);

        if(!await responseFile(res, filepath)) {
            await response404(res);
        }
    } catch(err) {
        console.log(err);
    }
});

const netifs = os.networkInterfaces();

for(let key in netifs) {
	const netinfo = netifs[key];
	if(netinfo) {
		console.log(key + ":");
		for(let key2 in netinfo) {
			console.log("\t" + netinfo[key2].address);
		}
	}
}

console.log("start webserver:" + port);
server.listen(port);
