from fastapi import FastAPI, HTTPException
from fastapi.responses import RedirectResponse
from pydantic import BaseModel
import shortuuid
from typing import Dict

app = FastAPI()

# In-memory storage for URLs
url_storage: Dict[str, str] = {}

class URLRequest(BaseModel):
    url: str

@app.post("/", status_code=201)
async def shorten_url(request: URLRequest):
    # Generate a unique short ID
    short_id = shortuuid.uuid()[:8]  # Use first 8 characters for brevity
    url_storage[short_id] = request.url
    return {"short_id": short_id, "short_url": f"http://127.0.0.1:8080/{short_id}"}

@app.get("/{short_id}", status_code=307)
async def redirect_url(short_id: str):
    original_url = url_storage.get(short_id)
    if not original_url:
        raise HTTPException(status_code=404, detail="Short URL not found")
    return RedirectResponse(url=original_url)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8080)