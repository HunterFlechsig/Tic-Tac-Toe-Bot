#!/bin/sh
set -eu

export PORT="${PORT:-80}"
export API_URL="${API_URL:-http://127.0.0.1:5000}"

rm -f /etc/nginx/sites-enabled/default
envsubst '${PORT} ${API_URL}' \
  < /etc/nginx/templates/default.conf.template \
  > /etc/nginx/sites-enabled/default

PORT=5000 /src/backend/.venv/bin/backend &
backend_pid=$!

i=0
while ! python -c "import urllib.request; urllib.request.urlopen('http://127.0.0.1:5000/health')" >/dev/null 2>&1; do
  if ! kill -0 "$backend_pid" 2>/dev/null; then
    echo "backend exited before it was ready" >&2
    exit 1
  fi
  i=$((i + 1))
  if [ "$i" -gt 60 ]; then
    echo "backend did not become ready" >&2
    exit 1
  fi
  sleep 0.5
done

exec nginx -g 'daemon off;'
